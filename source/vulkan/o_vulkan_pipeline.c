#include "o_vulkan_pipeline.h"

#include "Aero/a_memory.h"

#include "o_vulkan.h"
#include "o_vulkan_platform.h"
#include "o_vulkan_buffer_uniform.h"


/* Loads a shader file */
void* odin_vulkan_shader_load
(const char* path, aero_size_t* codeSize)
{
    /* Open the file in read and binary mode */
    FILE* shaderFile = fopen(path, "rb");

    /* Error check */
    if(!shaderFile)
    {
        LOG_ERROR("Could not load a shader file!");
    }

    /* Get the length of the file */
    int fileLength = 0;

    fseek(shaderFile, 0L, SEEK_END); /* Goto the end of the file */
    fileLength = ftell(shaderFile); /* Get the seek position */
    fseek(shaderFile, 0L, 0);  /* Seek back to beginning */
    *codeSize = fileLength;

    /* Allocate the file buffer */
    uint32_t* shaderBuffer = aero_malloc(sizeof(uint32_t) * fileLength);

    /* Read the file to the buffer */
    fread(shaderBuffer, sizeof(uint32_t), fileLength, shaderFile);

    fclose(shaderFile);

    return shaderBuffer;
}

/* Creates a vertex shader */
odin_shader_vertex* odin_vulkan_shader_vertex_create
(odin_data* data, void* code, aero_size_t codeSize)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Build the vextex shader module create info */
    VkShaderModuleCreateInfo vertex_shader_module_create_info = { 0 };
    vertex_shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertex_shader_module_create_info.pNext = NULL;
    vertex_shader_module_create_info.flags = 0;
    vertex_shader_module_create_info.codeSize = codeSize;
    vertex_shader_module_create_info.pCode = (uint32_t*)code;

    /* Create the vertex module */
    VkShaderModule module;
    VK_CHECK(vkCreateShaderModule(vulkan_data->device, &vertex_shader_module_create_info, NULL, &module), "Could not create a vertex shader module!");

    /* Create the odin_vertex_shader */
    odin_vulkan_shader_vertex* new_vertex_shader = (odin_vulkan_shader_vertex*)aero_malloc(sizeof(odin_vulkan_shader_vertex));
    new_vertex_shader->module = module;

    return (odin_shader_vertex*)new_vertex_shader;
}

/* Creates a fragment shader */
odin_shader_fragment* odin_vulkan_shader_fragment_create
(odin_data* data, void* code, aero_size_t codeSize)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Build thn vextex shader module create info */
    VkShaderModuleCreateInfo fragment_shader_module_create_info = { 0 };
    fragment_shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragment_shader_module_create_info.pNext = NULL;
    fragment_shader_module_create_info.flags = 0;
    fragment_shader_module_create_info.codeSize = codeSize;
    fragment_shader_module_create_info.pCode = (uint32_t*)code;

    /* Create the fragment module */
    VkShaderModule module;
    VK_CHECK(vkCreateShaderModule(vulkan_data->device, &fragment_shader_module_create_info, NULL, &module), "Could not create a fragment shader module!");

    /* Create the odin_fragment_shader */
    odin_vulkan_shader_fragment* new_fragment_shader = (odin_vulkan_shader_fragment*)aero_malloc(sizeof(odin_vulkan_shader_fragment));
    new_fragment_shader->module = module;

    return (odin_shader_fragment*)new_fragment_shader;
}

/* Creates a pipeline */
odin_pipeline* odin_vulkan_pipeline_create
(odin_data* data, odin_pipeline_descriptor_information* information, odin_shader_vertex* vertex_shader, odin_shader_fragment* fragment_shader)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Get the vulkan shaders */
    odin_vulkan_shader_vertex* vulkan_vertex_shader = (odin_vulkan_shader_vertex*)vertex_shader;
    odin_vulkan_shader_fragment* vulkan_fragment_shader = (odin_vulkan_shader_fragment*)fragment_shader;

    
    /* Get the descriptor set layouts */
    VkDescriptorSetLayout* descriptor_set_layouts = (VkDescriptorSetLayout*)aero_malloc(sizeof(VkDescriptorSetLayout) * information->uniforms_count);
    VkDescriptorSet* descriptor_sets = (VkDescriptorSet*)aero_malloc(sizeof(VkDescriptorSet) * information->uniforms_count);

    for(int i = 0; i < information->uniforms_count; i++)
    {
        odin_vulkan_buffer_uniform* vulkan_uniform = (odin_vulkan_buffer_uniform*)information->uniforms[i];
        aero_memcpy(&descriptor_set_layouts[i], sizeof(VkDescriptorSetLayout), &vulkan_uniform->descriptor_layout, sizeof(VkDescriptorSetLayout));
        aero_memcpy(&descriptor_sets[i], sizeof(VkDescriptorSet), &vulkan_uniform->descriptor_set, sizeof(VkDescriptorSet));
    }


    /* Build the pipeline layout create info */
    VkPipelineLayoutCreateInfo layout_create_info = { 0 };
    layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_create_info.pNext = NULL;
    layout_create_info.flags = 0;
    layout_create_info.setLayoutCount = information->uniforms_count;
    layout_create_info.pSetLayouts = descriptor_set_layouts;
    layout_create_info.pushConstantRangeCount = 0;
    layout_create_info.pPushConstantRanges = NULL;

    /* Create the pipeline layout */
    VkPipelineLayout layout = VK_NULL_HANDLE;
    VK_CHECK(vkCreatePipelineLayout(vulkan_data->device, &layout_create_info, NULL, &layout), "Could not create the pipeline layout!");

    /* Build the shader stages */
    #define SHADER_STAGE_COUNT 2
    
    VkPipelineShaderStageCreateInfo vertex_shader_stage_create_info = { 0 };
    vertex_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    vertex_shader_stage_create_info.pNext = NULL;
    vertex_shader_stage_create_info.flags = 0;
    vertex_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertex_shader_stage_create_info.module = vulkan_vertex_shader->module;
    vertex_shader_stage_create_info.pName = "main";
    vertex_shader_stage_create_info.pSpecializationInfo = NULL;

    VkPipelineShaderStageCreateInfo fragment_shader_stage_create_info = { 0 };
    fragment_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    fragment_shader_stage_create_info.pNext = NULL;
    fragment_shader_stage_create_info.flags = 0;
    fragment_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragment_shader_stage_create_info.module = vulkan_fragment_shader->module;
    fragment_shader_stage_create_info.pName = "main";
    fragment_shader_stage_create_info.pSpecializationInfo = NULL;

    VkPipelineShaderStageCreateInfo shader_stages[SHADER_STAGE_COUNT] = {vertex_shader_stage_create_info, fragment_shader_stage_create_info};

    /* Get the required vertex input state info */
    VkVertexInputBindingDescription* vertex_binding_descriptions = (VkVertexInputBindingDescription*)aero_malloc(sizeof(VkVertexInputBindingDescription) * odin_vulkan_vertex_assemblys.count);
    int vertex_attributes_count = 0;

    for(int i = 0; i < odin_vulkan_vertex_assemblys.count; i++)
    {
        /* Get the attributes count */
        odin_vulkan_vertex_assembly* vulkan_vertex_assembly = (odin_vulkan_vertex_assembly*)dynarr_get(&odin_vulkan_vertex_assemblys, i);
        vertex_attributes_count += vulkan_vertex_assembly->input_attribute_descriptions.count;
        
        /* Get the vertex binding descriptions */
        vertex_binding_descriptions[i] = vulkan_vertex_assembly->input_binding_description;
    }

    /* Build the vertex attributes */
    VkVertexInputAttributeDescription* vertex_attribute_descriptions = (VkVertexInputAttributeDescription*)aero_malloc(sizeof(VkVertexInputAttributeDescription) * vertex_attributes_count);

    for(int i = 0; i < odin_vulkan_vertex_assemblys.count; i++)
    {
        /* Get the vulkan vertex assembly */
        odin_vulkan_vertex_assembly* vulkan_vertex_assembly = (odin_vulkan_vertex_assembly*)dynarr_get(&odin_vulkan_vertex_assemblys, i);

        /* set the vertex attribute descriptions */
        for(int j = 0; j < vulkan_vertex_assembly->input_attribute_descriptions.count; j++)
        {
            aero_memcpy(&vertex_attribute_descriptions[j], sizeof(VkVertexInputAttributeDescription), ((VkVertexInputAttributeDescription*)dynarr_get(&vulkan_vertex_assembly->input_attribute_descriptions, j)), sizeof(VkVertexInputAttributeDescription));
        }
    }

    /* Build the vertex input state */
    VkPipelineVertexInputStateCreateInfo vertex_input_state = { 0 };
    vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_state.flags = 0;
    vertex_input_state.pNext = NULL;
    vertex_input_state.vertexBindingDescriptionCount = odin_vulkan_vertex_assemblys.count;
    vertex_input_state.pVertexBindingDescriptions = vertex_binding_descriptions;
    vertex_input_state.vertexAttributeDescriptionCount = vertex_attributes_count;
    vertex_input_state.pVertexAttributeDescriptions = vertex_attribute_descriptions;

    /* Build the input assembly state create info */
    VkPipelineInputAssemblyStateCreateInfo input_assembly_state = { 0 };
    input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state.pNext = NULL;
    input_assembly_state.flags = 0;
    input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_state.primitiveRestartEnable = VK_FALSE;

    /* Build the viewport state */
    VkViewport viewport = { 0 };
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)vulkan_data->platform_data->extent.width;
    viewport.height = (float)vulkan_data->platform_data->extent.height;
    viewport.maxDepth = 1.0f;
    viewport.minDepth = 0.0f;
    
    /* Build the scissor states */
    VkRect2D scissor = { 0 };
    scissor.offset = (VkOffset2D){0, 0};
    scissor.extent = vulkan_data->platform_data->extent;

    /* Define the viewport state */
    VkPipelineViewportStateCreateInfo viewport_state = { 0 };
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.pNext = NULL;
    viewport_state.flags = 0;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &scissor;

    /* Build the rasterization state */
    VkPipelineRasterizationStateCreateInfo rasterization_state = { 0 };
    rasterization_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_state.pNext = NULL;
    rasterization_state.flags = 0;
    rasterization_state.depthClampEnable = VK_FALSE;
    rasterization_state.rasterizerDiscardEnable = VK_FALSE;
    rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
    rasterization_state.lineWidth = 1.0f;
    rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterization_state.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterization_state.depthBiasEnable = VK_FALSE;
    rasterization_state.depthBiasConstantFactor = 0.0f;
    rasterization_state.depthBiasClamp = 0.0f;
    rasterization_state.depthBiasSlopeFactor = 0.0f;

    /* Build the multisampling state */
    VkPipelineMultisampleStateCreateInfo multisample_state = { 0 };    
    multisample_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state.pNext = NULL;
    multisample_state.flags = 0;
    multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisample_state.sampleShadingEnable = VK_FALSE;
    multisample_state.minSampleShading = 0.0f;
    multisample_state.pSampleMask = NULL;
    multisample_state.alphaToCoverageEnable = VK_FALSE;
    multisample_state.alphaToOneEnable = VK_FALSE;

    /* Build the depth stencil */
    VkPipelineDepthStencilStateCreateInfo depth_stencil_state = { 0 };
    depth_stencil_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil_state.pNext = NULL;
    depth_stencil_state.flags = 0;

    /* Build color blend options */
    VkPipelineColorBlendAttachmentState color_blend_attachment = { 0 };    
    color_blend_attachment.blendEnable = VK_TRUE;
    color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
    color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;
    color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    /* Build the color state */
    VkPipelineColorBlendStateCreateInfo color_blend_state = { 0 };
    color_blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state.pNext = NULL;
    color_blend_state.flags = 0;
    color_blend_state.logicOpEnable = VK_FALSE;
    color_blend_state.logicOp = VK_LOGIC_OP_COPY;
    color_blend_state.attachmentCount = 1;
    color_blend_state.pAttachments = &color_blend_attachment;
    color_blend_state.blendConstants[0] = 0.0f;
    color_blend_state.blendConstants[1] = 0.0f;
    color_blend_state.blendConstants[2] = 0.0f;
    color_blend_state.blendConstants[3] = 0.0f;

    /* Built the dynamic states */
    #define DYNAMIC_STATE_COUNT 3
    VkDynamicState dynamic_states[DYNAMIC_STATE_COUNT] = 
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    /* Bulid the dynamic state */
    VkPipelineDynamicStateCreateInfo dynamic_state = { 0 };    
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.pNext = NULL;
    dynamic_state.flags = 0;
    dynamic_state.dynamicStateCount = DYNAMIC_STATE_COUNT;
    dynamic_state.pDynamicStates = dynamic_states;

    /* Build the graphics pipeline create info */
    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = { 0 };
    graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_create_info.pNext = NULL;
    graphics_pipeline_create_info.flags = 0;
    graphics_pipeline_create_info.stageCount = SHADER_STAGE_COUNT;
    graphics_pipeline_create_info.pStages = shader_stages;
    graphics_pipeline_create_info.pVertexInputState = &vertex_input_state;
    graphics_pipeline_create_info.pInputAssemblyState = &input_assembly_state;
    graphics_pipeline_create_info.pTessellationState = NULL;
    graphics_pipeline_create_info.pViewportState = &viewport_state;
    graphics_pipeline_create_info.pRasterizationState = &rasterization_state;
    graphics_pipeline_create_info.pMultisampleState = &multisample_state;
    graphics_pipeline_create_info.pDepthStencilState = NULL;
    graphics_pipeline_create_info.pColorBlendState = &color_blend_state;
    graphics_pipeline_create_info.pDynamicState = &dynamic_state;
    graphics_pipeline_create_info.layout = layout;
    graphics_pipeline_create_info.renderPass = vulkan_data->render_pass;
    graphics_pipeline_create_info.subpass = 0;
    graphics_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
    graphics_pipeline_create_info.basePipelineIndex = -1;
    
    /* Create the graphics pipeline */
    VkPipeline pipeline = VK_NULL_HANDLE;
    VK_CHECK(vkCreateGraphicsPipelines(vulkan_data->device, VK_NULL_HANDLE, 1, &graphics_pipeline_create_info, NULL, &pipeline), "Could not create a graphics pipeline!");
    
    /* The shader modules are not needed anymore */
    vkDestroyShaderModule(vulkan_data->device, vulkan_vertex_shader->module, NULL);
    vkDestroyShaderModule(vulkan_data->device, vulkan_fragment_shader->module, NULL);

    /* Create the odin pipeline */
    odin_vulkan_pipeline* new_pipeline = (odin_vulkan_pipeline*)aero_malloc(sizeof(odin_vulkan_pipeline));
    new_pipeline->pipeline = pipeline;
    new_pipeline->layout = layout;
    new_pipeline->descriptor_set_count = information->uniforms_count;
    new_pipeline->descriptor_sets = descriptor_sets;

    return (odin_pipeline*)new_pipeline;
}

/* Destroys a pipeline */
void odin_vulkan_pipeline_destroy
(odin_data* data, odin_pipeline* pipeline)
{

}