#include "o_vulkan_input.h"


/* When a window is resized this should be called */
void odin_vulkan_input_on_window_resize
()
{
    g_odin_swapchain_out_of_date = true;
}

float odin_vulkan_get_window_width
()
{
    return g_odin_vulkan_platform_swapchain_extent.width;
}

float odin_vulkan_get_window_height
()
{
    return g_odin_vulkan_platform_swapchain_extent.height;
}