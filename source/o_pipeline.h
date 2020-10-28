#ifndef __ODIN_PIPELINE_H__
#define __ODIN_PIPELINE_H__


#include "stdint.h"

#include "o_uniform_buffer.h"
#include "o_image_2d.h"


/* A vertex shader that determines how vertices should act */
typedef struct odin_shader_vertex odin_shader_vertex;

/* A fragment shader that determines what pixels should look like */
typedef struct odin_shader_fragment odin_shader_fragment;

/* Descriptor information that tells the engine what resources a pipeline needs before hand */
typedef struct odin_pipeline_descriptor_information
{
    int uniforms_count;
    odin_uniform_buffer** uniforms;
    int image_count;
    odin_image_2d** images;
} odin_pipeline_descriptor_information;

/* A pipeline that binds shader programs */
typedef struct odin_pipeline odin_pipeline;


/* Loads a shader file */
void* (* odin_shader_load)
(const char* path, uint32_t* code_size);

/* Creates a vertex shader */
odin_shader_vertex* (* odin_shader_vertex_create)
(odin_data* data, void* code, uint32_t code_size);

/* Creates a fragment shader */
odin_shader_fragment* (* odin_shader_fragment_create)
(odin_data* data, void* code, uint32_t code_size);

/* Creates a pipeline */
odin_pipeline* (* odin_pipeline_create)
(odin_data* data, odin_pipeline_descriptor_information* information, odin_shader_vertex* vertex_shader, odin_shader_fragment* fragment_shader);

/* Destroys a pipeline */
void (* odin_pipeline_destroy)
(odin_data* data, odin_pipeline* pipeline);


#endif /* __ODIN_PIPELINE_H__ */