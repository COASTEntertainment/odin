#ifndef __ODIN_DRAW_H__
#define __ODIN_DRAW_H__


#include "o_render_device.h"
#include "o_draw_data.h"
#include "o_index_buffer.h"
#include "o_vertex_buffer.h"


/* Kindly lets the API know that its time to start drawing and create some useful objects. */
void ( *odin_draw_prepare)(odin_render_device render_device);

/* Tell the API to stop rendering and deletes no longer  */
void ( *odin_draw_done)(odin_render_device render_device);


/* Sets the current index buffer to be bound. */
void ( *odin_draw_command_set_index_buffer)(odin_render_device render_device, odin_draw_data draw_data, odin_index_buffer index_buffer);

/* Sets the vertex buffer to be bound. */
void ( *odin_draw_command_set_vertex_buffer)(odin_render_device render_device, odin_draw_data draw_data, odin_vertex_buffer vertex_buffer);


#endif /* __ODIN_DRAW_H__ */