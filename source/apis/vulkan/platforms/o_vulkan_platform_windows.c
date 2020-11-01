#include "odin/apis/vulkan/o_vulkan_platform.h"


const char **odin_vulkan_platform_get_validation_layers(uint32_t *validation_layers_count)
{

    static const uint32_t windows_validation_layers_count = 1;

    static const char *windows_validation_layers[] = 
    {
        "VK_LAYER_KHRONOS_validation"
    };


    *validation_layers_count = windows_validation_layers_count;
    return windows_validation_layers;

}

const char **odin_vulkan_platform_get_instance_extensions(uint32_t *instance_extensions_count)
{

    static const uint32_t windows_instance_extensions_count = 1;

    static const char *windows_instance_extensions[] = 
    {
        "VK_KHR_win32_surface"
    };


    *instance_extensions_count = windows_instance_extensions_count;
    return windows_instance_extensions;

}

const char **odin_vulkan_platform_get_device_extensions(uint32_t *device_extensions_count)
{

    static const uint32_t windows_device_extensions_count = 1;

    static const char *windows_device_extensions[] = 
    {
        "VK_LAYER_KHRONOS_validation"
    };


    *device_extensions_count = windows_device_extensions_count;
    return windows_device_extensions;

}