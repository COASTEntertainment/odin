#ifndef __ODIN_VULKAN_PLATFORM_WINDOWS_DATA_H__
#define __ODIN_VULKAN_PLATFORM_WINDOWS_DATA_H__


#include "../o_vulkan_platform_data.h"


#include "Windows.h"


typedef struct odin_vulkan_platform_windows_data
{
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    VkSurfaceFormatKHR format;
    VkExtent2D extent;

    uint32_t swapchain_images_count;
    VkImage* swapchain_images;
    odin_vulkan_buffer_frame** swapchain_frame_buffers;

    LPCSTR window_class_name;
    HWND hwnd;
} odin_vulkan_platform_windows_data;


#endif /* __ODIN_VULKAN_PLATFORM_WINDOWS_DATA_H__ */