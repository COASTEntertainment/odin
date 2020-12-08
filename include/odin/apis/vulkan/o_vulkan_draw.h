#ifndef __ODIN_VULKAN_DRAW_H__
#define __ODIN_VULKAN_DRAW_H__


#include "odin/o_draw.h"


void odin_vulkan_draw_prepare_window(odin_render_device render_device, odin_window window, odin_render_pass* present_pass);

void odin_vulkan_draw_frame_window(odin_render_device render_device, odin_window window, odin_draw_func draw_func, void* user_data);

void odin_vulkan_draw_done_window(odin_render_device render_device, odin_window window);


void odin_vulkan_draw_command_begin_render_pass(odin_render_device render_device, odin_draw_data draw_data, odin_render_pass render_pass, odin_framebuffer framebuffer);

void odin_vulkan_draw_command_end_render_pass(odin_render_device render_device, odin_draw_data draw_data, odin_render_pass render_pass);

void odin_vulkan_draw_command_bind_pipeline(odin_render_device render_device, odin_draw_data draw_data, odin_pipeline pipeline);

void odin_vulkan_draw_command_bind_vertex_buffer(odin_render_device render_device, odin_draw_data draw_data, odin_vertex_buffer vertex_buffer);

void odin_vulkan_draw_command_bind_index_buffer(odin_render_device render_device, odin_draw_data draw_data, odin_index_buffer index_buffer);

void odin_vulkan_draw_command_bind_texture(odin_render_device render_device, odin_draw_data draw_data);

void odin_vulkan_draw_command_indexed(odin_render_device render_device, odin_draw_data draw_data, int index_count);


#endif /* __ODIN_VULKAN_DRAW_H__ */