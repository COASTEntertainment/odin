#ifndef __ODIN_VULKAN_IMAGE_2D_H__
#define __ODIN_VULKAN_IMAGE_2D_H__


#include "../o_image_2d.h"

#include <vulkan/vulkan.h>
#include "vma.h"

/* A two dimensional image that can be used as a texture */
typedef struct odin_vulkan_image_2d
{
    VkImage image;
    VmaAllocation allocation;
    VkImageView image_view;
    VkSampler image_sampler;
} odin_vulkan_image_2d;


/* Creates an image from a file */
odin_image_2d* odin_vulkan_image_2d_create_and_load
(odin_data* data, const char* path);

/* Creates an image with data */ 
odin_image_2d* odin_vulkan_image_2d_create
(odin_data* data, int width, int height, odin_image_2d_tiling tiling_mode, void* image_data);

/* Destroys an image and its memory */
void odin_vulkan_image_2d_destroy
(odin_data* data, odin_image_2d* image);


#endif /* __ODIN_VULKAN_IMAGE_2D_H__ */