#ifndef __ODIN_VERTEX_BUFFER_H__
#define __ODIN_VERTEX_BUFFER_H__


#include "stdint.h"

#include "o_handle.h"
#include "o_data.h"
#include "o_vertex_assembly.h"


/* A buffer with vertices */
ODIN_DEFINE_HANDLE(odin_vertex_buffer) 

/* A dynamic buffer with vertices */
ODIN_DEFINE_HANDLE(odin_vertex_buffer_dynamic) 


/* Creates a buffer on the gpu with the specified vertices */
odin_vertex_buffer *( *odin_vertex_buffer_create)
(odin_data *data, odin_vertex_assembly *vertex_assembly, void *vertices, int32_t vertex_count); 

/* Destroys a vertex buffer */
void ( *odin_vertex_buffer_destroy)
(odin_data *data, odin_vertex_buffer *vertexBuffer); 

/* Creates a dynamic buffer on the gpu with the specified vertices */
odin_vertex_buffer_dynamic ( *odin_vertex_buffer_dynamic_create)
(odin_data *data, odin_vertex_assembly *vertexAssembly, void *vertices, int32_t vertex_count); 

/* Destroys a dynamic vertex buffer */
void ( *odin_vertex_buffer_dynamic_destroy)
(odin_data *data, odin_vertex_buffer_dynamic *vertex_buffer_dynamic);

/* Changes the data in a dynamic vertex buffer */
void ( *odin_vertex_buffer_dynamic_update)
(odin_data *data, odin_vertex_buffer_dynamic *vertex_buffer_dynamic, void *vertices, int32_t vertex_count); 


#endif /* __ODIN_VERTEX_BUFFER_H__ */