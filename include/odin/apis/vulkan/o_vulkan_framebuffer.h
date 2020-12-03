#ifndef __ODIN_VULKAN_FRAMEBUFFER_H__
#define __ODIN_VULKAN_FRAMEBUFFER_H__


#include "odin/o_framebuffer.h"
#include "odin/o_render_pass.h"
#include "odin/o_image.h"

#include <vulkan/vulkan.h>


typedef struct odin_vulkan_framebuffer
{

    VkFramebuffer framebuffer;

} odin_vulkan_framebuffer_t, *odin_vulkan_framebuffer;


void odin_vulkan_framebuffer_create(odin_render_device render_device, odin_framebuffer* framebuffer, int width, int height, odin_render_pass render_pass, int attachments_count, odin_image* attachments);

void odin_vulkan_framebuffer_destroy(odin_render_device render_device, odin_framebuffer framebuffer);


#endif /* __ODIN_VULKAN_FRAMEBUFFER_H__ */