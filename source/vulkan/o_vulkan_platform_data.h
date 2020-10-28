#ifndef __ODIN_VULKAN_PLATFORM_DATA_H__
#define __ODIN_VULKAN_PLATFORM_DATA_H__


#include <vulkan/vulkan.h>


typedef struct odin_vulkan_buffer_frame odin_vulkan_buffer_frame;


typedef struct odin_vulkan_platform_data
{
    
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    VkSurfaceFormatKHR format;
    VkExtent2D extent;

    uint32_t swapchain_images_count;
    VkImage *swapchain_images;
    odin_vulkan_buffer_frame *swapchain_frame_buffers;

} odin_vulkan_platform_data;


#endif /* __ODIN_VULKAN_PLATFORM_DATA_H__ */