#include "odin/apis/vulkan/o_vulkan_physical_device.h"

#include <stdlib.h>

#include <vulkan/vulkan.h>

#include <aero/a_memory.h>

#include "odin/o_log.h"
#include "odin/apis/vulkan/o_vulkan_render_device.h"
#include "odin/apis/vulkan/o_vulkan_platform.h"
#include "odin/apis/vulkan/o_vulkan_window.h"

#include "odin/apis/vulkan/vk_mem_alloc.h"


void odin_vulkan_get_physical_devices(odin_render_device render_device, int *physical_devices_count, odin_physical_device *physical_devices)
{

    /* Get the vulkan render device. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    /* Get the vulkan physical devices. */
    uint32_t vulkan_physical_devices_count = 0;
    vkEnumeratePhysicalDevices(vulkan_render_device->instance, &vulkan_physical_devices_count, NULL);

    if(!physical_devices)
    {
        *physical_devices_count = vulkan_physical_devices_count;
        return;
    }

    VkPhysicalDevice *vulkan_physical_devices = malloc(sizeof(VkPhysicalDevice) * vulkan_physical_devices_count);
    aero_memset(vulkan_physical_devices, sizeof(VkPhysicalDevice) * vulkan_physical_devices_count, 0);

    vkEnumeratePhysicalDevices(vulkan_render_device->instance, &vulkan_physical_devices_count, vulkan_physical_devices);

    *physical_devices_count = vulkan_physical_devices_count;


    /* Create the odin array of physical_devices. */
    for(uint32_t i = 0; i < vulkan_physical_devices_count; i++)
    {
        physical_devices[i] = (odin_physical_device)vulkan_physical_devices[i];
    }


}

void odin_vulkan_get_physical_device_properties(odin_render_device render_device, odin_physical_device physical_device, odin_physical_device_information_t* properties)
{

}

void odin_vulkan_set_physical_device(odin_render_device render_device, odin_physical_device physical_device, odin_window window)
{

    /* Get the vulkan render device. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    /* Get the vulkan window. */
    odin_vulkan_window vulkan_window = (odin_vulkan_window)window;

    /* Set the physical device. */
    vulkan_render_device->physical_device = (VkPhysicalDevice)physical_device;

    /* Set the main window. */
    vulkan_render_device->main_window = vulkan_window;


    /* Get physical device queues */
    uint32_t queue_family_count;
    vkGetPhysicalDeviceQueueFamilyProperties(vulkan_render_device->physical_device, &queue_family_count, NULL);

    /* Get the physical device queues array */
    VkQueueFamilyProperties* queue_properties = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(vulkan_render_device->physical_device, &queue_family_count, queue_properties);

    /* Get the individual required queues */
    for(uint32_t i = 0; i < queue_family_count; i++)
    {

        /* Get Graphics Queue */
        if(queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            vulkan_render_device->graphics_queue_index = i;
        }

        /* Get Present Queue */
        VkBool32 presentSupport;
        vkGetPhysicalDeviceSurfaceSupportKHR(vulkan_render_device->physical_device, i, vulkan_render_device->main_window->surface, &presentSupport);

        if(presentSupport)
        {
            vulkan_render_device->present_queue_index = i;
        }

        /* Get Compute Queue*/
        /*
        if(queueProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            g_odin_vulkan_compute_queue_family = i;
        }
        */
    }

    free(queue_properties);

    /* Create queue familys array */
    #define QUEUE_COUNT 2
    uint32_t queue_families[QUEUE_COUNT] = { vulkan_render_device->graphics_queue_index, vulkan_render_device->present_queue_index /* , g_odin_vulkan_compute_queue_family */  };



    uint32_t unique_queues_count = 0;
    uint32_t unique_queues[QUEUE_COUNT];

    /* Find the unique queues */
    for(int i = 0; i < QUEUE_COUNT; i++)
    {
        int j;
        for(j = 0; j < i; j++)
            if(queue_families[i] == queue_families[j])
                break;

        if(i == j)
        {
            /* Unique Queue Family */
            unique_queues_count++;
            unique_queues[i] = queue_families[i];
        }
    }

    /* Create device queue create infos */
    VkDeviceQueueCreateInfo* device_queue_create_infos = (VkDeviceQueueCreateInfo*)malloc(sizeof(VkDeviceQueueCreateInfo) * unique_queues_count);

    float queue_priorities = 1.0f;

    /* Set all of the queue create infos */
    for(uint32_t i = 0; i < unique_queues_count; i++)
    {
        /* Build the queue create info */
        VkDeviceQueueCreateInfo queue_create_info = { 0 };
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.pNext = NULL;
        queue_create_info.flags = 0;
        queue_create_info.queueFamilyIndex = unique_queues[i];
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priorities;
        
        /* Set the queue create info */
        device_queue_create_infos[i] = queue_create_info;
    }

    /* Get platform validation layers */
    uint32_t validation_layers_count = 0;
    const char** validation_layers = odin_vulkan_platform_get_validation_layers(&validation_layers_count);

    /* Get platform device extensions */
    uint32_t device_extensions_count = 0;
    const char** device_extensions = odin_vulkan_platform_get_device_extensions(&device_extensions_count);

    /* Specify features */
    VkPhysicalDeviceFeatures device_features = { 0 };
    device_features.samplerAnisotropy = VK_TRUE;

    /* Build the logical device create info */
    VkDeviceCreateInfo device_create_info = { 0 };
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pNext = NULL;
    device_create_info.flags = 0;
    device_create_info.queueCreateInfoCount = unique_queues_count;
    device_create_info.pQueueCreateInfos = device_queue_create_infos;
    device_create_info.enabledLayerCount = validation_layers_count;
    device_create_info.ppEnabledLayerNames = validation_layers;
    device_create_info.enabledExtensionCount = device_extensions_count;
    device_create_info.ppEnabledExtensionNames = device_extensions;
    device_create_info.pEnabledFeatures = &device_features;

    /* Make sure it was created */
    if(vkCreateDevice(vulkan_render_device->physical_device, &device_create_info, NULL, &vulkan_render_device->device) != VK_SUCCESS)
    {
        ODIN_ERROR("o_vulkan_physical_device.c", "Could not create the Vulkan logical device!");
    }

    /* Get Queues */
    vkGetDeviceQueue(vulkan_render_device->device, vulkan_render_device->graphics_queue_index, 0, &vulkan_render_device->graphics_queue);
    vkGetDeviceQueue(vulkan_render_device->device, vulkan_render_device->present_queue_index, 0, &vulkan_render_device->present_queue);
    /* vkGetDeviceQueue(g_odin_vulkan_device, g_odin_vulkan_compute_queue_family, 0, &g_odin_vulkan_compute_queue); */

    free(device_queue_create_infos);


    /* Create the memory allocator. */
    VmaAllocatorCreateInfo allocator_create_info = { 0 };
    allocator_create_info.flags                             = 0;
    allocator_create_info.physicalDevice                    = (VkPhysicalDevice)physical_device;
    allocator_create_info.device                            = vulkan_render_device->device;
    allocator_create_info.preferredLargeHeapBlockSize       = 0;
    allocator_create_info.pAllocationCallbacks              = NULL;
    allocator_create_info.pDeviceMemoryCallbacks            = NULL;
    allocator_create_info.frameInUseCount                   = 0;
    allocator_create_info.pHeapSizeLimit                    = NULL;
    allocator_create_info.pVulkanFunctions                  = NULL;
    allocator_create_info.pRecordSettings                   = NULL;
    allocator_create_info.instance                          = vulkan_render_device->instance;
    allocator_create_info.vulkanApiVersion                  = VK_API_VERSION_1_2;

    vmaCreateAllocator(&allocator_create_info, &vulkan_render_device->memory_allocator);


    /* Create the command pool. */
    VkCommandPoolCreateInfo command_pool_create_info = { 0 };
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.pNext = NULL;
    command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_create_info.queueFamilyIndex = vulkan_render_device->graphics_queue_index;

    vkCreateCommandPool(vulkan_render_device->device, &command_pool_create_info, NULL, &vulkan_render_device->command_pool);

    /* Create the main windows swapchain. */
    odin_vulkan_window_swapchain_create(vulkan_render_device, vulkan_window);

}