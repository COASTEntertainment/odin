#ifndef __ODIN_VULKAN_DRAW_H__
#define __ODIN_VULKAN_DRAW_H__


#include "odin/o_draw.h"


void odin_vulkan_draw_prepare(odin_render_device render_device);

void odin_vulkan_draw_frame(odin_render_device render_device, odin_draw_func draw_func, void* user_data);

void odin_vulkan_draw_done(odin_render_device render_device);


void odin_vulkan_draw_command_begin_render_pass(odin_render_device render_device, odin_draw_data draw_data, odin_render_pass render_pass, odin_framebuffer framebuffer);

void odin_vulkan_draw_command_end_render_pass(odin_render_device render_device, odin_draw_data draw_data, odin_render_pass render_pass);

void odin_vulkan_draw_command_bind_vertex_buffer(odin_render_device render_device, odin_draw_data draw_data, odin_vertex_buffer vertex_buffer);

void odin_vulkan_draw_command_bind_index_buffer(odin_render_device render_device, odin_draw_data draw_data, odin_index_buffer index_buffer);

void odin_vulkan_draw_command_bind_texture(odin_render_device render_device, odin_draw_data draw_data);


#endif /* __ODIN_VULKAN_DRAW_H__ */