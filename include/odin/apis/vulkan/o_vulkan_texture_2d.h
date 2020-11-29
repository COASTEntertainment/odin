#ifndef __ODIN_VULKAN_TEXTURE_2D_H__
#define __ODIN_VULKAN_TEXTURE_2D_H__


#include "odin/o_texture_2d.h"

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"


typedef struct odin_vulkan_texture_2d
{

    VkImage image;
    VmaAllocation image_allocation;
    VkImageView image_view;

} odin_vulkan_texture_2d_t, *odin_vulkan_texture_2d;


void odin_vulkan_texture_2d_create(odin_render_device render_device, odin_texture_2d* texture, odin_image_format format, int width, int height, int mip_levels, odin_texture_2d_samples samples, int32_t size, void *data);

void odin_vulkan_texture_2d_destroy(odin_render_device render_device, odin_texture_2d texture);

#endif /* __ODIN_VULKAN_TEXTURE_2D_H__ */