#ifndef __ODIN_VULKAN_PHYSICAL_DEVICE_H__
#define __ODIN_VULKAN_PHYSICAL_DEVICE_H__


#include "../o_physical_device.h"

#include <vulkan/vulkan.h>


typedef struct odin_vulkan_physical_device
{
    VkPhysicalDevice physical_device;
} odin_vulkan_physical_device;


/* Gets a list of physical device handles */
void odin_vulkan_get_physical_devices
(odin_data* data, int* devices_count, odin_physical_device** physical_devices);

/* Gets possibly required physical device info */
void odin_vulkan_physical_device_get_information
(odin_data* data, odin_physical_device* physical_device, odin_physical_device_information* physical_device_info);

/* Selects a physical device to use when rendering */
void odin_vulkan_physical_device_use
(odin_data* data, odin_physical_device* physical_device);


#endif /* __ODIN_VULKAN_PHYSICAL_DEVICE_H__ */