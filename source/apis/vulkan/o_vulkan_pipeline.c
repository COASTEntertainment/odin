#include "odin/apis/vulkan/o_vulkan_pipeline.h"

#include "odin/apis/vulkan/o_vulkan_render_device.h"


void **odin_vulkan_pipeline_shader_load_code(const char* path)
{

}

void odin_vulkan_pipeline_shader_create(odin_render_device render_device, odin_shader *shader, odin_shader_stage stage, void **shader_code)
{



}

void odin_vulkan_pipeline_create(odin_render_device render_device, odin_pipeline *pipeline, odin_shader vertex_shader, odin_shader fragment_shader)
{
    /* Get the vulkan render device. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;


    /* Allocate the odin pipeline. */
    odin_vulkan_pipeline vulkan_pipeline = malloc(sizeof(odin_vulkan_pipeline_t));
    *pipeline = (odin_pipeline)vulkan_pipeline;

    /* Get the vulkan shaders. */
    odin_vulkan_shader vulkan_vertex_shader = (odin_vulkan_shader)vertex_shader;
    odin_vulkan_shader vulkan_fragment_shader = (odin_vulkan_shader)fragment_shader;


    /* Create the pipeline layout. */
    VkPipelineLayoutCreateInfo pipeline_layout_create_info = { 0 };
    pipeline_layout_create_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_create_info.pNext                   = NULL;
    pipeline_layout_create_info.flags                   = 0;
    pipeline_layout_create_info.setLayoutCount          = 0;
    pipeline_layout_create_info.pSetLayouts             = NULL;
    pipeline_layout_create_info.pushConstantRangeCount  = 0;
    pipeline_layout_create_info.pPushConstantRanges     = NULL;

    vkCreatePipelineLayout(vulkan_render_device->device, &pipeline_layout_create_info, NULL, &vulkan_pipeline->layout);


    /* Create the shader stages. */
    VkPipelineShaderStageCreateInfo vertex_shader_pipeline_stage = { 0 };
    vertex_shader_pipeline_stage.sType                  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertex_shader_pipeline_stage.pNext                  = NULL;
    vertex_shader_pipeline_stage.flags                  = 0;
    vertex_shader_pipeline_stage.stage                  = VK_SHADER_STAGE_VERTEX_BIT;
    vertex_shader_pipeline_stage.module                 = vulkan_vertex_shader->module;
    vertex_shader_pipeline_stage.pName                  = "main";
    vertex_shader_pipeline_stage.pSpecializationInfo    = NULL;

    VkPipelineShaderStageCreateInfo fragment_shader_pipeline_stage = { 0 };
    fragment_shader_pipeline_stage.sType                  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragment_shader_pipeline_stage.pNext                  = NULL;
    fragment_shader_pipeline_stage.flags                  = 0;
    fragment_shader_pipeline_stage.stage                  = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragment_shader_pipeline_stage.module                 = vulkan_fragment_shader->module;
    fragment_shader_pipeline_stage.pName                  = "main";
    fragment_shader_pipeline_stage.pSpecializationInfo    = NULL;


    VkPipelineShaderStageCreateInfo shader_pipeline_stages[] = 
    {   
        vertex_shader_pipeline_stage,
        fragment_shader_pipeline_stage
    }


    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = { 0 };
    graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_create_info.pNext = NULL;
    graphics_pipeline_create_info.flags = 0;
    graphics_pipeline_create_info.stageCount = 2;
    graphics_pipeline_create_info.pStages = shader_pipeline_stages;



}

void odin_vulkan_pipeline_destroy(odin_render_device render_device, odin_pipeline pipeline)
{

}
