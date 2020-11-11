#ifndef __ODIN_INDEX_BUFFER_H__
#define __ODIN_INDEX_BUFFER_H__


#include "o_render_device.h"


/** \brief How a vertex buffer should be used when drawing. */
typedef struct odin_index_buffer_t *odin_index_buffer;


/** \brief Creates an index buffer and uploads its information to the GPU. */
void odin_index_buffer_create(odin_render_device render_device, odin_index_buffer* index_buffer, uint32_t* indices_data);

/** \brief Destroys an index buffer and removes the information from the gpu. */
void odin_index_buffer_destroy(odin_render_device render_device, odin_index_buffer index_buffer);


#endif /* __ODIN_INDEX_BUFFER_H__ */