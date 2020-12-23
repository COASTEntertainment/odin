#ifndef __ODIN_VULKAN_DRAW_DATA_H__
#define __ODIN_VULKAN_DRAW_DATA_H__


#include "odin/o_draw_data.h"
#include "odin/apis/vulkan/o_vulkan_window.h"

#include <vulkan/vulkan.h>


typedef struct odin_vulkan_draw_data
{

    VkCommandBuffer command_buffer;
    odin_vulkan_window vulkan_window;

} odin_vulkan_draw_data_t, *odin_vulkan_draw_data;


#endif /* __ODIN_VULKAN_DRAW_DATA_H__ */