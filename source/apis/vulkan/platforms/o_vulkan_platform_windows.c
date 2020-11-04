#include "odin/apis/vulkan/o_vulkan_platform.h"

#include <Windows.h>

#include "odin/apis/vulkan/o_vulkan_window.h"


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


void odin_vulkan_platform_initialize(odin_vulkan_render_device render_device)
{

    /* Create the windows window class */
    WNDCLASSEX window_class = { 0 };
    window_class.style              = 0;
    window_class.lpfnWndProc        = DefWindowProc; /* Window procedure must be defined by an input layer! */
    window_class.cbClsExtra         = 0;
    window_class.cbWndExtra         = 0;
    window_class.hInstance          = GetModuleHandle(NULL);
    window_class.hIcon              = NULL;
    window_class.hCursor            = NULL;
    window_class.hbrBackground      = NULL;
    window_class.lpszMenuName       = NULL;
    window_class.lpszClassName      = "ODIN_VULKAN_WINDOW_CLASS";

    RegisterClassEx(&window_class);

}

void odin_vulkan_platform_terminate(odin_vulkan_render_device render_device)
{

}