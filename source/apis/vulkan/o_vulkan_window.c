#include "odin/apis/vulkan/o_vulkan_window.h"

#include <Windows.h>
#include <dwmapi.h>

#include <vulkan/vulkan.h>

#include "aero/a_memory.h"

#include "odin/o_log.h"
#include "odin/apis/vulkan/o_vulkan_render_device.h"
#include "odin/o_platform_internal.h"


void odin_vulkan_window_create(odin_render_device render_device, odin_window* window, odin_input_device input_device, const char* title, int x, int y, int width, int height, odin_window_style style)
{
    AERO_NULL_CHECK(render_device);
    AERO_NULL_CHECK(input_device);


    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    odin_vulkan_window vulkan_window = AERO_NEW(vulkan_window, 1);
    aero_memset(vulkan_window, sizeof(vulkan_window), 0);
    

    odin_platform_create_native_window(&vulkan_window->native_window, (odin_window)vulkan_window, input_device, title, width, height);
    odin_platform_create_vulkan_surface(vulkan_window->native_window, vulkan_render_device->instance, &vulkan_window->surface);


    if(vulkan_render_device->device)
    {
        odin_vulkan_window_swapchain_create(vulkan_render_device, vulkan_window);
    }

    *window = (odin_window)vulkan_window;
}

void odin_vulkan_window_destroy(odin_render_device render_device, odin_window window)
{
    AERO_NULL_CHECK(render_device);
    AERO_NULL_CHECK(window);

    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;
    odin_vulkan_window vulkan_window = (odin_vulkan_window)window;


    odin_vulkan_window_swapchain_destroy(vulkan_render_device, vulkan_window);

    vkDestroySurfaceKHR(vulkan_render_device->instance, vulkan_window->surface, NULL);

    odin_platform_destroy_native_window(vulkan_window->native_window);
}

void odin_vulkan_window_swapchain_create(odin_vulkan_render_device vulkan_render_device, odin_vulkan_window vulkan_window)
{


    VkSurfaceCapabilitiesKHR surface_capabilities = { 0 };
    VkResult res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan_render_device->physical_device, vulkan_window->surface, &surface_capabilities);

    /* Get the min image count. */
    vulkan_window->image_count = surface_capabilities.minImageCount + 1;
    if (vulkan_window->image_count > surface_capabilities.maxImageCount)
    {
        vulkan_window->image_count = surface_capabilities.maxImageCount;
    }
    
    uint32_t surface_formats_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan_render_device->physical_device, vulkan_window->surface, &surface_formats_count, NULL);

    VkSurfaceFormatKHR* surface_formats = AERO_NEW(VkSurfaceFormatKHR, surface_formats_count);
    aero_memset(surface_formats, sizeof(VkSurfaceFormatKHR) * surface_formats_count, 0);

    vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan_render_device->physical_device, vulkan_window->surface, &surface_formats_count, surface_formats);

    bool found_surface_format = false;
    for(uint32_t i = 0; i < surface_formats_count; i++)
    {
        if(surface_formats[i].format == VK_FORMAT_R8G8B8A8_SRGB && surface_formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            found_surface_format = true;
            vulkan_window->surface_format = surface_formats[i];
            break;
        }
    }

    if(!found_surface_format)
    {
        vulkan_window->surface_format = surface_formats[0];
    }

    if (surface_capabilities.currentExtent.width != UINT32_MAX) {
        vulkan_window->surface_extent = surface_capabilities.currentExtent;
    } else {
        int width;
        int height;

        odin_platform_get_native_window_size(vulkan_window->native_window, &width, &height);

        VkExtent2D actual_extent = {width, height};
        actual_extent.width = max(surface_capabilities.minImageExtent.width, min(surface_capabilities.maxImageExtent.width, width));
        actual_extent.height = max(surface_capabilities.minImageExtent.height, min(surface_capabilities.maxImageExtent.height, height));

        vulkan_window->surface_extent = actual_extent;
    }


    uint32_t queue_family_indices[] = {vulkan_render_device->graphics_queue_index, vulkan_render_device->present_queue_index};

    uint32_t present_modes_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan_render_device->physical_device, vulkan_window->surface, &present_modes_count, NULL);

    VkPresentModeKHR* present_modes = malloc(sizeof(VkPresentModeKHR) * present_modes_count);
    aero_memset(present_modes, sizeof(VkPresentModeKHR) * present_modes_count, 0);

    vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan_render_device->physical_device, vulkan_window->surface, &present_modes_count, present_modes);

    bool found_present_mode = false;

    for(uint32_t i = 0; i < present_modes_count; i++)
    {
        if(present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            vulkan_window->present_mode = present_modes[i];
            found_present_mode = true;
            break;
        }
    }

    /* If no present mode is found FIFO or VSync can be used. */
    if(!found_present_mode)
        vulkan_window->present_mode = VK_PRESENT_MODE_FIFO_KHR; 


    VkSwapchainCreateInfoKHR swapchain_create_info = { 0 };
    swapchain_create_info.sType                     = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.pNext                     = NULL;
    swapchain_create_info.flags                     = 0;
    swapchain_create_info.surface                   = vulkan_window->surface;
    swapchain_create_info.minImageCount             = vulkan_window->image_count;
    swapchain_create_info.imageFormat               = vulkan_window->surface_format.format;
    swapchain_create_info.imageColorSpace           = vulkan_window->surface_format.colorSpace;
    swapchain_create_info.imageExtent               = vulkan_window->surface_extent;
    swapchain_create_info.imageArrayLayers          = 1;
    swapchain_create_info.imageUsage                = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_create_info.imageSharingMode          = vulkan_render_device->graphics_queue_index != vulkan_render_device->present_queue_index ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;
    swapchain_create_info.queueFamilyIndexCount     = vulkan_render_device->graphics_queue_index != vulkan_render_device->present_queue_index ? 2 : 0;
    swapchain_create_info.pQueueFamilyIndices       = vulkan_render_device->graphics_queue_index != vulkan_render_device->present_queue_index ? queue_family_indices : NULL;
    swapchain_create_info.preTransform              = surface_capabilities.currentTransform;
    swapchain_create_info.compositeAlpha            = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info.presentMode               = vulkan_window->present_mode;
    swapchain_create_info.clipped                   = VK_FALSE;
    swapchain_create_info.oldSwapchain              = NULL;
    
    if(vkCreateSwapchainKHR(vulkan_render_device->device, &swapchain_create_info, NULL, &vulkan_window->swapchain) != VK_SUCCESS)
    {
        ODIN_ERROR("o_vulkan_window_windows.c", "Could not create a window's swapchain!");
    }

    
    vkGetSwapchainImagesKHR(vulkan_render_device->device, vulkan_window->swapchain, &vulkan_window->image_count, NULL);

    vulkan_window->images = malloc(sizeof(VkImage) * vulkan_window->image_count);

    vkGetSwapchainImagesKHR(vulkan_render_device->device, vulkan_window->swapchain, &vulkan_window->image_count, vulkan_window->images);
}


void odin_vulkan_window_swapchain_destroy(odin_vulkan_render_device render_device, odin_vulkan_window window)
{

    vkDestroySwapchainKHR(render_device->device, window->swapchain, NULL);
}

