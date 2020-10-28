#ifndef __ENGINE_ODIN_H__
#define __ENGINE_ODIN_H__


#include "o_data.h"
#include "o_physical_device.h"
#include "o_vertex_assembly.h"
#include "o_model.h"
#include "o_buffer_vertex.h"
#include "o_buffer_index.h"
#include "o_buffer_command.h"
#include "o_pipeline.h"
#include "o_image_2d.h"
#include "o_draw.h"
#include "o_input.h"
#include "o_platform.h"






/* Begins the rendering initalizing */
void (* odin_init)
(odin_data** data); 

/* Stops rendering and frees up memory */
void (* odin_free)
(odin_data* data); 


#endif /* __ENGINE_ODIN_H__ */