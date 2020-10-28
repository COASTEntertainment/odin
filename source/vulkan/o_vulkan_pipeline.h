#ifndef __ODIN_VULKAN_PIPELINE_H__
#define __ODIN_VULKAN_PIPELINE_H__


#include "../o_pipeline.h"

#include <vulkan/vulkan.h>


/* A vertex shader that determines how vertices should act */
typedef struct odin_vulkan_shader_vertex
{

    VkShaderModule module;

} odin_vulkan_shader_vertex;

/* A fragment shader that determines what pixels should look like */
typedef struct odin_vulkan_shader_fragment 
{

    VkShaderModule module;

} odin_vulkan_shader_fragment;
 
/* A pipeline that binds shader programs */
typedef struct odin_vulkan_pipeline
{

    VkPipelineLayout    layout;
    VkPipeline          pipeline;
    int                 descriptor_set_count;
    VkDescriptorSet    *descriptor_sets;

} odin_vulkan_pipeline;


/* Loads a shader file */
void* odin_vulkan_shader_load
(const char *path, uint32_t *codeSize);

/* Creates a vertex shader */
odin_shader_vertex *odin_vulkan_shader_vertex_create
(odin_data *data, void *code, uint32_t codeSize);

/* Creates a fragment shader */
odin_shader_fragment *odin_vulkan_shader_fragment_create
(odin_data *data, void *code, uint32_t codeSize);

/* Creates a pipeline */
odin_pipeline *odin_vulkan_pipeline_create
(odin_data *data, odin_pipeline_descriptor_information *information, odin_shader_vertex *vertex_shader, odin_shader_fragment *fragment_shader);

/* Destroys a pipeline */
void odin_vulkan_pipeline_destroy
(odin_data *data, odin_pipeline *pipeline);


#endif /* __ODIN_VULKAN_PIPELINE_H__ */