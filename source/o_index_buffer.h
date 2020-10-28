#ifndef __ODIN_INDEX_BUFFER_H__
#define __ODIN_INDEX_BUFFER_H__


#include "stdint.h"

#include "o_data.h"


/* A buffer with vertex indices */
typedef struct odin_index_buffer odin_index_buffer;

/* A dynamic buffer with vertex indices */
typedef struct odin_index_buffer_dynamic odin_index_buffer_dynamic;


/* Creates a buffer on the GPU with vertex indices */
odin_index_buffer* (* odin_index_buffer_create)
(odin_data* data, uint32_t* indices, int32_t indexCount); 

/* Destroys a index buffer */
void (* odin_index_buffer_destroy)
(odin_data* data, odin_index_buffer* indexBuffer);

/* Creates a dynamic buffer on the GPU with vertex indices */
odin_index_buffer_dynamic* (* odin_index_buffer_dynamic_create)
(odin_data* data, uint32_t* indices, int32_t index_count); 

/* Destroys a dynamic index buffer */
void (* odin_index_buffer_dynamic_destroy)
(odin_data* data, odin_index_buffer_dynamic* index_buffer_dynamic); 

/* Changes the data in a dynamic index buffer */
void (* odin_index_buffer_dynamic_update)
(odin_data* data, odin_index_buffer_dynamic* index_buffer_dynamic, uint32_t* indices, int32_t index_count); 



#endif /* __ODIN_INDEX_BUFFER__H__ */