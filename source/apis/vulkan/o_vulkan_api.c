#include "odin/apis/vulkan/o_vulkan_api.h"

#include "odin/odin.h"
#include "odin/apis/vulkan/o_vulkan.h"


void odin_vulkan_api_load()
{

    odin_initialize = odin_vulkan_initialize;
    odin_terminate = odin_vulkan_terminate;

    odin_get_monitors = odin_vulkan_get_monitors;
    odin_window_create = odin_vulkan_window_create;
    odin_window_destroy = odin_vulkan_window_destroy;

    odin_get_physical_devices = odin_vulkan_get_physical_devices;
    odin_get_physical_device_properties = odin_vulkan_get_physical_device_properties;
    odin_set_physical_device = odin_vulkan_set_physical_device;

    odin_vertex_assembly_create = odin_vulkan_vertex_assembly_create;
    odin_vertex_assembly_destroy = odin_vulkan_vertex_assembly_destroy;
    odin_vertex_assembly_describe_element = odin_vulkan_vertex_assembly_describe_element;

}