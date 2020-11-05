#ifndef __ODIN_VULKAN_PHYSICAL_DEVICE_H__
#define __ODIN_VULKAN_PHYSICAL_DEVICE_H__


#include "odin/o_physical_device.h"

#include <vulkan/vulkan.h>


typedef struct VkPhysicalDevice odin_vulkan_physical_device;


/** \brief Gets the graphics cards that are on the system. */
void odin_vulkan_get_physical_devices(odin_render_device render_device, int *physical_devices_count, odin_physical_device *physical_devices);

/** \brief Gets the properties of a physical device. */
void odin_vulkan_get_physical_device_properties(odin_render_device render_device, odin_physical_device physical_device, odin_physical_device_information_t* properties);

/** \brief Sets a physical device to be used at runtime when rendering */
void odin_vulkan_set_physical_device(odin_render_device render_device, odin_physical_device physical_device, odin_window window);


#endif /* __ODIN_VULKAN_PHYSICAL_DEVICE_H__ */