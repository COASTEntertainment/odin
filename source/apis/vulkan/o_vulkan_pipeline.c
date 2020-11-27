#include "odin/apis/vulkan/o_vulkan_pipeline.h"

#include "aero/a_memory.h"

#include "odin/apis/vulkan/o_vulkan_render_device.h"
#include "odin/apis/vulkan/o_vulkan_vertex_assembly.h"



void **odin_vulkan_pipeline_shader_load_code(const char* path)
{
    return NULL;
}

void odin_vulkan_pipeline_shader_create(odin_render_device render_device, odin_shader *shader, odin_shader_stage stage, void **shader_code)
{



}

void odin_vulkan_pipeline_create(odin_render_device render_device, odin_pipeline *pipeline, odin_shader vertex_shader, odin_shader fragment_shader, int vertex_assemblys_count, odin_vertex_assembly* vertex_assemblys)
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
    };


    uint32_t total_attributes_count = 0;

    /* Get total attributes count. */
    for(int i = 0; i < vertex_assemblys_count; i++)    
        total_attributes_count += ((odin_vulkan_vertex_assembly)vertex_assemblys[i])->attributes_count;

    VkVertexInputBindingDescription*    vertex_bindings         = malloc(sizeof(VkVertexInputBindingDescription) * vertex_assemblys_count); 
    VkVertexInputAttributeDescription*  attribute_descriptions  = malloc(sizeof(VkVertexInputAttributeDescription) * total_attributes_count);

    int attributes_iterator = 0;

    for(int i = 0; i < vertex_assemblys_count; i++)
    {
        odin_vulkan_vertex_assembly vulkan_vertex_assembly = (odin_vulkan_vertex_assembly)vertex_assemblys[i];

        /* Set the vertex bindings */
        aero_memcpy(&vertex_bindings[i], sizeof(VkVertexInputBindingDescription), &vulkan_vertex_assembly->binding_description, sizeof(VkVertexInputBindingDescription));

        for(uint32_t j = 0; j < vulkan_vertex_assembly->attributes_count; j++)
        {
            /* Copy the vertex attributes. */
            aero_memcpy(&attribute_descriptions[attributes_iterator], sizeof(VkVertexInputAttributeDescription), &vulkan_vertex_assembly->attributes[j], sizeof(VkVertexInputAttributeDescription));
            attributes_iterator++;
        }
    }

    /* Create the vertex input state. */
    VkPipelineVertexInputStateCreateInfo vertex_pipeline_state = { 0 };
    vertex_pipeline_state.sType                             = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_pipeline_state.pNext                             = NULL;
    vertex_pipeline_state.flags                             = 0;
    vertex_pipeline_state.vertexBindingDescriptionCount     = vertex_assemblys_count;
    vertex_pipeline_state.pVertexBindingDescriptions        = vertex_bindings;
    vertex_pipeline_state.vertexAttributeDescriptionCount   = total_attributes_count;
    vertex_pipeline_state.pVertexAttributeDescriptions      = attribute_descriptions;


    /* Create the input assembly state. */
    VkPipelineInputAssemblyStateCreateInfo input_assembly_state = { 0 };
    input_assembly_state.sType                      = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state.pNext                      = NULL;
    input_assembly_state.flags                      = 0;
    input_assembly_state.topology                   = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_state.primitiveRestartEnable     = VK_FALSE;


    /* Create the tesselation state. */
    VkPipelineTessellationStateCreateInfo tessellation_state = { 0 };
    tessellation_state.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tessellation_state.pNext = NULL;
    tessellation_state.flags = 0;
    tessellation_state.patchControlPoints = 0;


    /* Create the viewport state. */
    VkPipelineViewportStateCreateInfo viewport_state = { 0 };
    viewport_state.sType            = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.pNext            = NULL;
    viewport_state.flags            = 0;
    viewport_state.viewportCount    = 0;
    viewport_state.pViewports       = NULL;
    viewport_state.scissorCount     = 0;
    viewport_state.pViewports       = NULL;


    /* Create the rasterization state. */
    VkPipelineRasterizationStateCreateInfo rasterization_state = { 0 };
    rasterization_state.sType                       = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_state.pNext                       = NULL;
    rasterization_state.flags                       = 0;
    rasterization_state.depthBiasEnable             = VK_FALSE;
    rasterization_state.rasterizerDiscardEnable     = VK_TRUE;
    rasterization_state.polygonMode                 = VK_POLYGON_MODE_FILL;
    rasterization_state.cullMode                    = VK_CULL_MODE_BACK_BIT;
    rasterization_state.frontFace                   = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterization_state.depthBiasEnable             = VK_FALSE;
    rasterization_state.depthBiasConstantFactor     = 1.0f;
    rasterization_state.depthBiasClamp              = 1.0f;
    rasterization_state.depthBiasSlopeFactor        = 1.0f;
    rasterization_state.lineWidth                   = 1.0f;


    /* Create the multisample state. */
    VkPipelineMultisampleStateCreateInfo multisample_state = { 0 };
    multisample_state.sType                     = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state.pNext                     = NULL;
    multisample_state.flags                     = 0;
    multisample_state.rasterizationSamples      = VK_FALSE;
    multisample_state.sampleShadingEnable       = VK_FALSE;
    multisample_state.minSampleShading          = 0.0f;
    multisample_state.pSampleMask               = NULL;
    multisample_state.alphaToCoverageEnable     = VK_FALSE;
    multisample_state.alphaToOneEnable          = VK_FALSE;


    /* Create the depth stencil state. */
    VkPipelineDepthStencilStateCreateInfo depth_stencil_state = { 0 };
    depth_stencil_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil_state.pNext = NULL;
    depth_stencil_state.flags = 0;
    depth_stencil_state.depthTestEnable = VK_FALSE;
    depth_stencil_state.depthWriteEnable = VK_FALSE;
    depth_stencil_state.depthCompareOp = VK_COMPARE_OP_NEVER;
    depth_stencil_state.depthBoundsTestEnable = VK_FALSE;
    depth_stencil_state.stencilTestEnable = VK_FALSE;
    depth_stencil_state.front = (VkStencilOpState){ 0 };
    depth_stencil_state.back = (VkStencilOpState){ 0 };
    depth_stencil_state.minDepthBounds = 0.0f;
    depth_stencil_state.maxDepthBounds = 0.0f;


    /* Create the color blend state. */
    VkPipelineColorBlendStateCreateInfo color_blend_state = { 0 };
    color_blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state.pNext = NULL;
    color_blend_state.flags = 0;
    color_blend_state.logicOpEnable = VK_FALSE;
    color_blend_state.logicOp = VK_LOGIC_OP_CLEAR;
    color_blend_state.attachmentCount = 1;
    color_blend_state.pAttachments = NULL;
    color_blend_state.blendConstants[0] = 1.0f;
    color_blend_state.blendConstants[1] = 1.0f;
    color_blend_state.blendConstants[2] = 1.0f;
    color_blend_state.blendConstants[3] = 1.0f;
    


    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = { 0 };
    graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_create_info.pNext = NULL;
    graphics_pipeline_create_info.flags = 0;
    graphics_pipeline_create_info.stageCount = 2;
    graphics_pipeline_create_info.pStages = shader_pipeline_stages;
    graphics_pipeline_create_info.pVertexInputState = &vertex_pipeline_state;
    graphics_pipeline_create_info.pInputAssemblyState = &input_assembly_state;
    graphics_pipeline_create_info.pTessellationState = &tessellation_state;
    graphics_pipeline_create_info.pViewportState = &viewport_state;
    graphics_pipeline_create_info.pRasterizationState = &rasterization_state;
    graphics_pipeline_create_info.pMultisampleState = &multisample_state;
    graphics_pipeline_create_info.pDepthStencilState = &depth_stencil_state;
    graphics_pipeline_create_info.pColorBlendState = &color_blend_state;


}

void odin_vulkan_pipeline_destroy(odin_render_device render_device, odin_pipeline pipeline)
{

}
