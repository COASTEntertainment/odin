#ifndef __ODIN_VULKAN_TEXTURE_2D_H__
#define __ODIN_VULKAN_TEXTURE_2D_H__


#include "odin/o_texture_2d.h"

#include <vulkan/vulkan.h>


typedef struct odin_vulkan_texture_2d
{

    VkImage image;
    VkImageView image_view;

} odin_vulkan_texture_2d_t, *odin_vulkan_texture_2d;



#endif /* __ODIN_VULKAN_TEXTURE_2D_H__ */