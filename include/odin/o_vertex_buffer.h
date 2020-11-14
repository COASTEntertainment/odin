#ifndef __ODIN_VERTEX_BUFFER_H__
#define __ODIN_VERTEX_BUFFER_H__


#include "o_render_device.h"
#include "o_vertex_assembly.h"


typedef struct odin_vertex_buffer_t *odin_vertex_buffer;


/** \brief Creates a vertex buffer and uploads its information to the GPU. */
void ( *odin_vertex_buffer_create)(odin_render_device render_device, odin_vertex_buffer* vertex_buffer, odin_vertex_assembly vertex_assembly, int vertices_count, void* vertex_data);

/** \brief Destroys a vertex buffer and deletes it on the GPU. */
void ( *odin_vertex_buffer_destroy)(odin_render_device render_device, odin_vertex_buffer vertex_buffer);


#endif /* __ODIN_VERTEX_BUFFER_H__ */