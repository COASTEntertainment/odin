#ifndef __ODIN_DRAW_H__
#define __ODIN_DRAW_H__


#include "o_data.h"
#include "o_command_buffer.h"
#include "o_vertex_buffer.h"
#include "o_index_buffer.h"
#include "o_pipeline.h"


typedef void (* odin_draw_func)(odin_data *render, odin_buffer_command *command_buffer);


/* When all required render objects are created we can prepare for rendering */
void ( *odin_draw_prepare)
(odin_data *data); 

/* Changes the drawing function */
void ( *odin_draw_set_function)
(odin_data *data, odin_draw_func draw_func);

/* Changes the clear color */
void ( *odin_draw_set_clear_color_vec4f)
(odin_data *data, float *color);

/* Specifies which vertex buffer to use this draw */
void ( *odin_draw_set_buffer_vertex)
(odin_data *data, odin_buffer_command *command_buffer, odin_buffer_vertex *vertex_buffer); 

/* Specifies which index buffer to use this draw */
void ( *odin_draw_set_buffer_index)
(odin_data *data, odin_buffer_command *command_buffer, odin_buffer_index *index_buffer); 

/* Binds a pipeline for rendering */
void ( *odin_draw_set_pipeline)
(odin_data *data, odin_buffer_command *command_buffer, odin_pipeline *pipeline);

/* Submits the commants to drawing the set objects to the gpu */
void ( *odin_draw_indexed)
(odin_data *data, odin_buffer_command *command_buffer, uint32_t index_count); 

/* When rendering is to stop */
void ( *odin_draw_done)
(odin_data *data); 

/* Tells the api when to draw a frame */
void ( *odin_draw_frame)
(odin_data *data);


#endif /* __ODIN_DRAW_H__ */