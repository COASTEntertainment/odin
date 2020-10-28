#ifndef __ODIN_BUFFER_UNIFORM_H__
#define __ODIN_BUFFER_UNIFORM_H__


#include "stdint.h"

#include "o_data.h"


/* Which stage the uniform buffer is to be used at */
typedef enum odin_uniform_buffer_stage_type
{

    odin_uniform_buffer_stage_type_vertex = 0,
    odin_uniform_buffer_stage_type_fragment = 1

} odin_uniform_buffer_stage_type;


typedef struct odin_uniform_buffer odin_uniform_buffer;


/* Creates a uniform buffer object */
odin_uniform_buffer* (* odin_uniform_buffer_create)
(odin_data* data, odin_uniform_buffer_stage_type stage, uint32_t size);

/* Destroys a uniform buffer */
void (* odin_uniform_buffer_destroy)
(odin_data* data, odin_uniform_buffer* uniform);

/* Changes the data in the uniform */
void (* odin_uniform_buffer_update)
(odin_data* data, odin_uniform_buffer* uniform, void* uniform_data);


#endif /* __ODIN_BUFFER_UNIFORM_H__ */