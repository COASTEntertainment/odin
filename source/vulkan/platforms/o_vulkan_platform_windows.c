#include "../o_vulkan_platform.h"

#define NOMINMAX
#include <Windows.h>
#include <dwmapi.h>

#include <vulkan/vulkan.h>

#include "aero/core.h"
#include "aero/dynarr.h"
#include "Aero/a_memory.h"
#include "aero/Log.h"

#include "../o_vulkan.h"

#include "../o_vulkan_framebuffer.h"
#include "o_vulkan_platform_windows_data.h"


const char* ODIN_VULKAN_WINDOW_CLASS_NAME = "vulkan_window_class";


void odin_vulkan_platform_init
(odin_data* data)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Create the platform data */
    odin_vulkan_platform_windows_data* platform_data = (odin_vulkan_platform_windows_data*)aero_malloc(sizeof(odin_vulkan_platform_windows_data));
    vulkan_data->platform_data = (odin_vulkan_platform_data*)platform_data;

    /* Create the window class info */
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    //wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    wc.lpszClassName = ODIN_VULKAN_WINDOW_CLASS_NAME;

    /* Register the window class */
    RegisterClass(&wc);

    /* Create the window */
    platform_data->hwnd = CreateWindowEx(
        0, /* Window Styles */
        ODIN_VULKAN_WINDOW_CLASS_NAME, /* Class name */
        g_applicationName, /* Window title */
        WS_OVERLAPPEDWINDOW, /* Window style */

        /* Size and position */
        CW_USEDEFAULT, CW_USEDEFAULT, ODIN_PLATFORM_DEFAULT_WINDOW_WIDTH, ODIN_PLATFORM_DEFAULT_WINDOW_HEIGHT,

        NULL, /* Parent window */
        NULL, /* Menu */
        GetModuleHandle(NULL), /* Instance handle */
        NULL /* Additional application data */
    );

    if(platform_data->hwnd == NULL)
    {
        LOG_ERROR("Could not create window!");
    }

    ShowWindow(platform_data->hwnd, SW_SHOW);

    /* Build the win32 surface create info */
    VkWin32SurfaceCreateInfoKHR surface_create_info = { 0 };
    surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surface_create_info.pNext = NULL;
    surface_create_info.flags = 0;
    surface_create_info.hinstance = GetModuleHandle(NULL);
    surface_create_info.hwnd = platform_data->hwnd;

    /* Create the windows surface */
    VK_CHECK(vkCreateWin32SurfaceKHR(vulkan_data->instance, &surface_create_info, NULL, &platform_data->surface), "Could not create surface!");
}

void odin_vulkan_platform_free
(odin_data* data)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Get the vulkan windows platform data */
    odin_vulkan_platform_windows_data* platform_data = (odin_vulkan_platform_windows_data*)vulkan_data->platform_data;

    /* Destroy the swapchain */
    odin_vulkan_platform_destroy_swapchain(vulkan_data);

    /* Destroy the surface */
    vkDestroySurfaceKHR(vulkan_data->instance, platform_data->surface, NULL);

    /* Destroy the window */
    DestroyWindow(platform_data->hwnd);
}

const char** odin_platform_vulkan_get_validation_layers(uint32_t* count)
{
    /* Define the static unchanging variables */
    static const char* validation_layers[] = 
    { 
        "VK_LAYER_KHRONOS_validation" 
    };

    static const int validation_layers_count = 1;

    /* Return the static unchanging variables */
    *count = validation_layers_count;
    return validation_layers;
}

const char** odin_platform_vulkan_get_instance_extensions(uint32_t* count)
{

    /* Define the static unchanging variables */    
#if !defined(NDEBUG)
    static const char* instance_extensions[] = { 
        "VK_KHR_surface", 
        "VK_KHR_win32_surface",
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };
    static const int instance_extensions_count = 3;
#else
    static const char* instance_extensions[] = { 
        "VK_KHR_surface", 
        "VK_KHR_win32_surface"
    };
    static const int  instance_extensions_count = 2;
#endif 

    /* Return the static unchanging variables */
    *count = instance_extensions_count;
    return instance_extensions;
}

const char** odin_platform_vulkan_get_device_extensions(uint32_t* count)
{
    /* Define the static unchanging variables */
    static const char* device_extensions[] = 
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    static const char device_extensions_count = 1;

    /* Return the static unchanging variables */
    *count = device_extensions_count;
    return device_extensions;
}


void odin_platform_vulkan_recreate_swapchain
(odin_vulkan_data* vulkan_data)
{

    /* Get the platform data */
    odin_vulkan_platform_windows_data* platform_data = (odin_vulkan_platform_windows_data*)vulkan_data->platform_data;

    /* Get surface capabilities */
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan_data->physical_device, platform_data->surface, &capabilities);

    /* Get all of the formats count */
    uint32_t surface_formats_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan_data->physical_device, platform_data->surface, &surface_formats_count, NULL);

    /* Get all of the formats */
    VkSurfaceFormatKHR* surface_formats = (VkSurfaceFormatKHR*)aero_malloc(sizeof(VkSurfaceFormatKHR) * surface_formats_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan_data->physical_device, platform_data->surface, &surface_formats_count, surface_formats);

    /* Choose the wanted format or else... */
    bool foundFormat = false;
    for(uint32_t i = 0; i < surface_formats_count; i++)
    {
        if(surface_formats[i].format == VK_FORMAT_R8G8B8A8_SRGB && surface_formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            /* This is the suggested and most portable.. probably */
            platform_data->format = surface_formats[i];
            foundFormat = true;
            break;
        }
    }

    /* Just go with first format if the suggested one was not found.. probably */
    if(!foundFormat)
    {
        platform_data->format = surface_formats[0];
    }

    /* Cleanup */
    aero_free(surface_formats);

    /* Choose swap extent */
    if(capabilities.currentExtent.width != UINT32_MAX)
    {
        vulkan_data->platform_data->extent = capabilities.currentExtent;
    }
    else
    { 
        /* Get the window window size */
        RECT frame;
        DwmGetWindowAttribute(platform_data->hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &frame, sizeof(RECT));

        //rect should be `0, 0, 1280, 1024`
        //frame should be `7, 0, 1273, 1017`
        
        VkExtent2D actualExtent = {
            (uint32_t) frame.right - frame.left,
            (uint32_t) frame.bottom - frame.top
        };

        actualExtent.width = aether_max(capabilities.minImageExtent.width, aether_min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = aether_max(capabilities.minImageExtent.height, aether_min(capabilities.maxImageExtent.height, actualExtent.height));

        platform_data->extent = actualExtent;
    }

    /* Get the image counts */
    vulkan_data->image_count = capabilities.minImageCount + 1;

    if(capabilities.maxImageCount > 0 && vulkan_data->image_count > capabilities.maxImageCount)
    {
        vulkan_data->image_count = capabilities.maxImageCount;
    }


    /* Choose image sharing mode */
    VkSharingMode sharingMode = vulkan_data->graphics_queue_index != vulkan_data->present_queue_index ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;

    /* Choose queue family indices */
    uint32_t queueFamilyIndicesArr[] = { vulkan_data->graphics_queue_index, vulkan_data->present_queue_index };
    
    uint32_t queueFamilyIndicesCount = vulkan_data->graphics_queue_index != vulkan_data->present_queue_index ? 2 : 0;
    uint32_t* queueFamilyIndices = vulkan_data->graphics_queue_index != vulkan_data->present_queue_index ? queueFamilyIndicesArr : NULL;
    
    /* Choose present mode */
    {
        /* Get present modes */
        uint32_t present_mode_count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan_data->physical_device, platform_data->surface, &present_mode_count, NULL);

        VkPresentModeKHR* present_modes = (VkPresentModeKHR*)aero_malloc(sizeof(VkPresentModeKHR) * present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan_data->physical_device, platform_data->surface, &present_mode_count, present_modes);

        /* Choose present mode */
        bool found_present_mode = false;
        for(uint32_t i = 0; i < present_mode_count; i++)
        {
            /* Mailbox is most prefered because it provides speed with minimal buffering */
            if(present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                vulkan_data->present_mode = present_modes[i];
                found_present_mode = true;
                break;
            }
        }

        /* Present mode FiFo is always going to be there */
        if(!found_present_mode)
            vulkan_data->present_mode = VK_PRESENT_MODE_FIFO_KHR;

        /* Cleanup */
        aero_free(present_modes);
    }

    /* Build the swapchain create info */
    VkSwapchainCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.surface = platform_data->surface;
    createInfo.minImageCount = vulkan_data->image_count;
    createInfo.imageFormat = platform_data->format.format;
    createInfo.imageColorSpace = platform_data->format.colorSpace;
    createInfo.imageExtent = platform_data->extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = sharingMode;
    createInfo.queueFamilyIndexCount = queueFamilyIndicesCount;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = vulkan_data->present_mode;
    createInfo.clipped = VK_FALSE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    
    /* Create the swapchain */
    VK_CHECK(vkCreateSwapchainKHR(vulkan_data->device, &createInfo, NULL, &platform_data->swapchain), "Could not create a swapchain!");


    /* Get the swapchain image count */
    vkGetSwapchainImagesKHR(vulkan_data->device, platform_data->swapchain, &vulkan_data->image_count, NULL);

    /* Allocate swapchain image array */
    VkImage* swapchain_images = (VkImage*)aero_malloc(sizeof(VkImage) * vulkan_data->image_count);
    vkGetSwapchainImagesKHR(vulkan_data->device, platform_data->swapchain, &vulkan_data->image_count, swapchain_images);

    /* Allocate the swapchain framebuffers */
    vulkan_data->framebuffers = (odin_vulkan_framebuffer**)aero_malloc(sizeof(odin_vulkan_framebuffer*) * vulkan_data->image_count);

    /* Create the framebuffers */
    for(uint32_t i = 0; i < vulkan_data->image_count; i++)
    {
        vulkan_data->framebuffers[i] = odin_vulkan_framebuffer_create_from_image(vulkan_data, vulkan_data->render_pass, platform_data->extent, swapchain_images[i]);
    }


    /* Build the command buffers allocation info */
    VkCommandBufferAllocateInfo command_buffer_allocate_info = { 0 };
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.pNext = NULL;
    command_buffer_allocate_info.commandPool = vulkan_data->command_pool;
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount = vulkan_data->image_count;

    /* Allocate the command buffers */
    vulkan_data->command_buffers = (odin_vulkan_buffer_command*)aero_malloc(sizeof(odin_vulkan_buffer_command) * vulkan_data->image_count);

    /* Allocate the vulkan command buffers */
    for(uint32_t i = 0; i < vulkan_data->image_count; i++)
    {
        VK_CHECK(vkAllocateCommandBuffers(vulkan_data->device, &command_buffer_allocate_info, &vulkan_data->command_buffers[i].buffer), "Could not create the command buffers!");
    }
}

/* Destroys all swapchain information */
void odin_vulkan_platform_destroy_swapchain
(odin_vulkan_data* data)
{
    /* Get the odin_vulkan_data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Wait for device idle */
    vkDeviceWaitIdle(vulkan_data->device);

    /* Free the command buffers */
    for(uint32_t i = 0; i < vulkan_data->image_count; i++)
    {
        vkFreeCommandBuffers(vulkan_data->device, vulkan_data->command_pool, 1, &vulkan_data->command_buffers[i].buffer);
    }

    /* Destroy the swapchain frame buffers */
    for(uint32_t i = 0; i < vulkan_data->image_count; i++)
    {
        odin_vulkan_buffer_frame_destroy_from_image((odin_buffer_frame*)vulkan_data->framebuffers[i]);
    }

    aero_free(vulkan_data->framebuffers);
}

void* odin_vulkan_get_input_window_handle(odin_data* data)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Get the vulkan windows platform data */
    odin_vulkan_platform_windows_data* platform_data = (odin_vulkan_platform_windows_data*)vulkan_data->platform_data;

    /* Return the windows window handle */
    return platform_data->hwnd;
}

