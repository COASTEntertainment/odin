#ifndef __ODIN_VULKAN_FRAMEBUFFER_H__
#define __ODIN_VULKAN_FRAMEBUFFER_H__


#include <vulkan/vulkan.h>
#include "o_vulkan_data.h"


/* A framebuffer that describes pixels */
typedef struct odin_vulkan_framebuffer
{
    VkFramebuffer framebuffer;
    VkImage image;
    VkImageView image_view;
} odin_vulkan_framebuffer;


/* Creates a vulkan framebuffer */
odin_vulkan_framebuffer* odin_vulkan_framebuffer_create(odin_vulkan_data* vulkan_data, VkRenderPass render_pass, VkExtent2D extent);

/* Creates a vulkan framebuffer from an image */
odin_vulkan_framebuffer* odin_vulkan_framebuffer_create_from_image(odin_vulkan_data* vulkan_data, VkRenderPass render_pass, VkExtent2D extent, VkImage image);

/* Destroys a framebuffer */
void odin_vulkan_framebuffer_destroy(odin_vulkan_framebuffer* framebuffer);

/* Destroys a framebuffer created using the from image function (doesnt destroy the image) */
void odin_vulkan_framebuffer_destroy_from_image(odin_vulkan_framebuffer* framebuffer);


#endif /* __ODIN_VULKAN_FRAMEBUFFER_H__ */