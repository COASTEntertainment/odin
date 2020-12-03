#ifndef __ODIN_VULKAN_IMAGE_H__
#define __ODIN_VULKAN_IMAGE_H__


#include "odin/o_image.h"

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"


typedef struct odin_vulkan_image
{

    int                 o_width;
    int                 o_height;
    int                 o_depth;
    int                 o_mip_levels;
    odin_image_format   o_format;

    VkImage                 image;
    VmaAllocation           image_allocation;
    VkFormat                format;
    VkImageView             image_view;
    VkSampleCountFlagBits   samples;

} odin_vulkan_image_t, *odin_vulkan_image;


void odin_vulkan_image_create(odin_render_device render_device, odin_image* image, odin_image_format format, int width, int height, int depth, int mip_levels, odin_image_samples samples);

void odin_vulkan_image_upload_data(odin_render_device render_device, odin_image image, void* data);

void odin_vulkan_image_destroy(odin_render_device render_device, odin_image image);

#endif /* __ODIN_VULKAN_IMAGE_H__ */