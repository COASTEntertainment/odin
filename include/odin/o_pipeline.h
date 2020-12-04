#ifndef __ODIN_PIPELINE_H__
#define __ODIN_PIPELINE_H__


#include "o_render_device.h"
#include "o_vertex_assembly.h"
#include "o_render_pass.h"


/** \brief Data about a shaders code. */
typedef struct odin_shader_code *odin_shader_code;

/** \brief A shader controls parts of the render pipeline. */
typedef struct odin_shader *odin_shader;

/** \brief A pipeline controls pixels and what they look like on the screen. */
typedef struct odin_pipeline *odin_pipeline;

/** \brief Shader stages determine which stage a shader should be. */
typedef enum odin_shader_stage 
{

    odin_shader_stage_vertex = 0,
    odin_shader_stage_fragment = 0

} odin_shader_stage;


/** \brief Loads shader code on an API basis. */
void ( *odin_pipeline_shader_load_code)(odin_shader_code* shader_code, const char* path);

/** \brief Creates a shader for a pipeline. */
void ( *odin_pipeline_shader_create)(odin_render_device render_device, odin_shader *shader, odin_shader_stage stage, odin_shader_code shader_code);

/** \brief Creates a pipeline from shaders.  */
void ( *odin_pipeline_create)(odin_render_device render_device, odin_pipeline *pipeline, odin_shader vertex_shader, odin_shader fragment_shader, odin_render_pass render_pass, int vertex_assemblys_count, odin_vertex_assembly* vertex_assemblys);

/** \brief Destroys a pipeline from use. */
void ( *odin_pipeline_destroy)(odin_render_device render_device, odin_pipeline pipeline);


#endif /* __ODIN_PIPELINE_H__ */