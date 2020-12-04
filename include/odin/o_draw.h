#ifndef __ODIN_DRAW_H__
#define __ODIN_DRAW_H__


#include "o_render_device.h"
#include "o_draw_data.h"
#include "o_index_buffer.h"
#include "o_vertex_buffer.h"
#include "o_pipeline.h"
#include "o_framebuffer.h"
#include "o_render_pass.h"



/* Lets the API know that its time to start drawing and create some useful objects. */
void ( *odin_draw_prepare)(odin_render_device render_device);

/* Draw the frame. */
void ( *odin_draw_frame)(odin_render_device odin_render_device, odin_draw_func draw_func);

/* Tell the API to stop rendering and deletes no longer  */
void ( *odin_draw_done)(odin_render_device render_device);


/* Begins a render pass, drawing to a framebuffer. */
void ( *odin_draw_command_begin_render_pass)(odin_render_device render_device, odin_draw_data draw_data, odin_render_pass render_pass, odin_framebuffer framebuffer);

/* Ends a render pass and stops drawing the framebuffer. */
void ( *odin_draw_command_end_render_pass)(odin_render_device render_device, odin_draw_data draw_data, odin_render_pass render_pass);

/* Sets the pipeline to be using. */
void ( *odin_draw_command_set_pipeline)(odin_render_device render_device, odin_draw_data draw_data, odin_pipeline pipeline);

/* Sets the vertex buffer to be bound. */
void ( *odin_draw_command_set_vertex_buffer)(odin_render_device render_device, odin_draw_data draw_data, odin_vertex_buffer vertex_buffer);

/* Sets the current index buffer to be bound. */
void ( *odin_draw_command_set_index_buffer)(odin_render_device render_device, odin_draw_data draw_data, odin_index_buffer index_buffer);

/* Draws a vertex buffer and index buffer. */
void ( *odin_draw_indexed)(odin_render_device render_device, odin_draw_data draw_data, int index_count);


#endif /* __ODIN_DRAW_H__ */