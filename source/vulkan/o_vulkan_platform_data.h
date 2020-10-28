#ifndef __ODIN_VULKAN_PLATFORM_DATA_H__
#define __ODIN_VULKAN_PLATFORM_DATA_H__


#include <vulkan/vulkan.h>


typedef struct odin_vulkan_platform_data
{
    
    VkSurfaceKHR            surface;
    VkSwapchainKHR          swapchain;
    VkSurfaceFormatKHR      format;
    VkPresentModeKHR        present_mode;
    VkExtent2D              extent;

    uint32_t                image_count;
    VkImage                 *swapchain_images;
    VkFramebuffer           *framebuffers;

    bool                    swapchain_out_of_date;

} odin_vulkan_platform_data;


#endif /* __ODIN_VULKAN_PLATFORM_DATA_H__ */