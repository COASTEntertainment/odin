#include "o_vulkan_physical_device.h"

#include "o_vulkan.h"
#include "o_vulkan_platform.h"

#include "vma.h"


/* Gets a list of physical device handles */
void odin_vulkan_get_physical_devices
(odin_data* data, int* devices_count, odin_physical_device** physical_devices)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Get physical device count */
    uint32_t physical_device_count = 0;
    vkEnumeratePhysicalDevices(vulkan_data->instance, &physical_device_count, NULL);

    /* Get all of the physical devices */
    VkPhysicalDevice* temp_physical_devices = malloc(sizeof(VkPhysicalDevice) * physical_device_count);
    vkEnumeratePhysicalDevices(vulkan_data->instance, &physical_device_count, temp_physical_devices);

    /* Set the physical device count */
    *devices_count = physical_device_count;

    /* Create the physical device array */
    odin_vulkan_physical_device* vulkan_physical_devices = malloc(sizeof(odin_vulkan_physical_device) * physical_device_count);
    memset(vulkan_physical_devices, sizeof(odin_vulkan_physical_device) * physical_device_count, 0);

    /* Add physical devices to array */
    for(uint32_t i = 0; i < physical_device_count; i++)
    {
        vulkan_physical_devices[i].physical_device = temp_physical_devices[i];
    }

    /* Cleanup the temp_physical_devices */
    free(temp_physical_devices);

    /* Return with the new array */
    *physical_devices = (odin_physical_device*)vulkan_physical_devices;
}

/* Gets possibly required physical device info */
void odin_vulkan_physical_device_get_information
(odin_data* data, odin_physical_device* physical_device, odin_physical_device_information* physical_device_info)
{
    /* Get the vulkan physical device */
    odin_vulkan_physical_device* vulkan_physical_device = (odin_vulkan_physical_device*)physical_device;

    /* Get vulkan physical device info */
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(vulkan_physical_device->physical_device, &properties);

    /* Set information */
    memset(physical_device_info->name, VK_MAX_PHYSICAL_DEVICE_NAME_SIZE, '\0');
    strcpy(physical_device_info->name, VK_MAX_PHYSICAL_DEVICE_NAME_SIZE, properties.deviceName);
}

#define QUEUE_COUNT 2

/* Selects a physical device to use when rendering */
void odin_vulkan_physical_device_use
(odin_data* data, odin_physical_device* physical_device)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Get the vulkan physical device */
    odin_vulkan_physical_device* vulkan_physical_device = (odin_vulkan_physical_device*)physical_device;

    /* Sets odin_vulkan_physical_device */
    vulkan_data->physical_device = vulkan_physical_device->physical_device;

    /* Get physical device queues */
    uint32_t queue_family_count;
    vkGetPhysicalDeviceQueueFamilyProperties(vulkan_data->physical_device, &queue_family_count, NULL);

    /* Get the physical device queues array */
    VkQueueFamilyProperties* queue_properties = (VkQueueFamilyProperties*)aero_malloc(sizeof(VkQueueFamilyProperties) * queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(vulkan_data->physical_device, &queue_family_count, queue_properties);

    /* Get the individual required queues */
    for(uint32_t i = 0; i < queue_family_count; i++)
    {

        /* Get Graphics Queue */
        if(queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            vulkan_data->graphics_queue_index = i;
        }

        /* Get Present Queue */
        VkBool32 presentSupport;
        vkGetPhysicalDeviceSurfaceSupportKHR(vulkan_data->physical_device, i, vulkan_data->platform_data->surface, &presentSupport);

        if(presentSupport)
        {
            vulkan_data->present_queue_index = i;
        }

        /* Get Compute Queue*/
        /*
        if(queueProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            g_odin_vulkan_compute_queue_family = i;
        }
        */
    }

    /* Create queue familys array */
    uint32_t queue_families[QUEUE_COUNT] = { vulkan_data->graphics_queue_index, vulkan_data->present_queue_index /* , g_odin_vulkan_compute_queue_family */  };



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
    VkDeviceQueueCreateInfo* device_queue_create_infos = (VkDeviceQueueCreateInfo*)aero_malloc(sizeof(VkDeviceQueueCreateInfo) * unique_queues_count);

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
    VkPhysicalDeviceFeatures deviceFeatures = { 0 };
    deviceFeatures.samplerAnisotropy = VK_TRUE;

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
    device_create_info.pEnabledFeatures = &deviceFeatures;

    /* Make sure it was created */
    VK_CHECK(vkCreateDevice(vulkan_data->physical_device, &device_create_info, NULL, &vulkan_data->device), "Could not create vulkan device!")

    /* Get Queues */
    vkGetDeviceQueue(vulkan_data->device, vulkan_data->graphics_queue_index, 0, &vulkan_data->graphics_queue);
    vkGetDeviceQueue(vulkan_data->device, vulkan_data->present_queue_index, 0, &vulkan_data->present_queue);
    /* vkGetDeviceQueue(g_odin_vulkan_device, g_odin_vulkan_compute_queue_family, 0, &g_odin_vulkan_compute_queue); */


    /* Build the vulkan memory allocator create info */
    VmaAllocatorCreateInfo allocatorInfo = { 0 };
    allocatorInfo.instance = vulkan_data->instance;
    allocatorInfo.physicalDevice = vulkan_data->physical_device;
    allocatorInfo.device = vulkan_data->device;

    /* Create the vulkan memory allocator */
    vmaCreateAllocator(&allocatorInfo, &vulkan_data->memory_allocator);

    /* Bulid the command pool create info */
    VkCommandPoolCreateInfo command_pool_create_info = { 0 };
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.pNext = NULL;
    command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_create_info.queueFamilyIndex = vulkan_data->graphics_queue_index;

    /* Create the command pool */
    VK_CHECK(vkCreateCommandPool(vulkan_data->device, &command_pool_create_info, NULL, &vulkan_data->command_pool), "Could not create the command pool!");

    /* Build the descriptor pool sizes */
    #define DESCRIPTOR_POOL_SIZES_COUNT 1
    VkDescriptorPoolSize descriptor_pool_sizes[DESCRIPTOR_POOL_SIZES_COUNT] = {
        {
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            1000
        }
    };

    /* Build the descriptor pool create info */
    VkDescriptorPoolCreateInfo descriptor_pool_create_info = { 0 };
    descriptor_pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool_create_info.pNext = NULL;
    descriptor_pool_create_info.flags = 0;
    descriptor_pool_create_info.maxSets = 1000;
    descriptor_pool_create_info.poolSizeCount = DESCRIPTOR_POOL_SIZES_COUNT;
    descriptor_pool_create_info.pPoolSizes = descriptor_pool_sizes;

    /* Create the descriptor pool */
    VK_CHECK(vkCreateDescriptorPool(vulkan_data->device, &descriptor_pool_create_info, NULL, &vulkan_data->descriptor_pool), "Could not create a descriptor pool!");

    /* Initalize defaults */
    //odin_init_defaults();

}


