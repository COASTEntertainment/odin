#include "odin/apis/vulkan/o_vulkan_physical_device.h"

#include <stdlib.h>

#include <vulkan/vulkan.h>

#include <aero/a_memory.h>

#include "odin/apis/vulkan/o_vulkan_render_device.h"


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
    for(int i = 0; i < vulkan_physical_devices_count; i++)
    {
        physical_devices[i] = (odin_physical_device)vulkan_physical_devices[i];
    }


}

void odin_vulkan_get_physical_device_properties(odin_render_device render_device, odin_physical_device physical_device, odin_physical_device_information_t* properties)
{

}

void odin_vulkan_set_physical_device(odin_render_device render_device, odin_physical_device physical_device)
{

}