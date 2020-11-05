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


