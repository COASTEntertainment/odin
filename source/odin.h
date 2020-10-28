#ifndef __ENGINE_ODIN_H__
#define __ENGINE_ODIN_H__


#include "o_data.h"
#include "o_physical_device.h"
#include "o_vertex_assembly.h"
#include "o_vertex_buffer.h"
#include "o_index_buffer.h"
#include "o_pipeline.h"
#include "o_image_2d.h"
#include "o_draw.h"
#include "o_input.h"
#include "o_platform.h"


typedef struct odin_initalization_information
{

    const char* application_name;
    int application_version_major, application_version_minor, application_version_patch;
    const char* engine_name;
    int engine_version_major, engine_version_minor, engine_version_patch;

} odin_initalization_information;


/* Begins the rendering initalizing */
void (* odin_init)
(odin_data** data, odin_initalization_information* initalization_info); 

/* Stops rendering and frees up memory */
void (* odin_free)
(odin_data* data); 


#endif /* __ENGINE_ODIN_H__ */