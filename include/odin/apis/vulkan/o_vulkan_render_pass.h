#ifndef __ODIN_VULKAN_RENDER_PASS_H__
#define __ODIN_VULKAN_RENDER_PASS_H__


#include "odin/o_render_pass.h"

#include <vulkan/vulkan.h>


typedef struct odin_vulkan_render_pass
{

    int test;

} odin_vulkan_render_pass_t, *odin_vulkan_render_pass;



void odin_vulkan_render_pass_create(odin_render_device render_device, odin_render_pass* render_pass);


#endif /* __ODIN_VULKAN_RENDER_PASS_H__ */