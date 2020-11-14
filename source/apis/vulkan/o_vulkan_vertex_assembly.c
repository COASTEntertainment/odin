#include "odin/apis/vulkan/o_vulkan_vertex_assembly.h"

#include <vulkan/vulkan.h>

#include "aero/a_memory.h"


/** \brief Creates a vertex assembly. */
void odin_vulkan_vertex_assembly_create(odin_render_device render_device, odin_vertex_assembly* vertex_assembly, int binding, int32_t vertex_size, int elements_count)
{

    /* Allocate the vertex assembly. */
    odin_vulkan_vertex_assembly vulkan_vertex_assembly = malloc(sizeof(odin_vulkan_vertex_assembly_t));
    aero_memset(vulkan_vertex_assembly, sizeof(odin_vulkan_vertex_assembly_t), 0);
    
    *vertex_assembly = (odin_vertex_assembly)vulkan_vertex_assembly;

    vulkan_vertex_assembly->binding_description.binding = binding;
    vulkan_vertex_assembly->binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    vulkan_vertex_assembly->binding_description.stride = vertex_size;

    /* Allocate the vertex attributes. */
    vulkan_vertex_assembly->attributes = malloc(sizeof(VkVertexInputAttributeDescription) * elements_count);
    aero_memset(vulkan_vertex_assembly->attributes, sizeof(VkVertexInputAttributeDescription) * elements_count, 0);

}

/** \brief Destroys a vertex assembly. */
void odin_vulkan_vertex_assembly_destroy(odin_render_device render_device, odin_vertex_assembly vertex_assembly)
{

    /* Get the vulkan vertex assembly. */
    odin_vulkan_vertex_assembly vulkan_vertex_assembly = (odin_vulkan_vertex_assembly)vertex_assembly;

    free(vulkan_vertex_assembly->attributes);
    free(vulkan_vertex_assembly);
    
    vertex_assembly = NULL;

}


void odin_vulkan_vertex_format_to_vulkan(odin_vertex_element_format element_format, int32_t* element_size, VkFormat* format)
{
    
    switch(element_format)
    {
    
        case odin_vertex_element_format_float:
            *element_size = sizeof(float);
            *format = VK_FORMAT_R32_SFLOAT;
            break;

        case odin_vertex_element_format_int32:
            *element_size = sizeof(int32_t);
            *format = VK_FORMAT_R32_SINT;
            break;

        case odin_vertex_element_format_int64:
            *element_size = sizeof(int64_t);
            *format = VK_FORMAT_R64_SINT;
            break;

        case odin_vertex_element_format_vec2f:
            *element_size = sizeof(float[2]);
            *format = VK_FORMAT_R32G32_SFLOAT;
            break;
        
        case odin_vertex_element_format_vec2i32:
            *element_size = sizeof(int32_t[2]);
            *format = VK_FORMAT_R32G32_SINT;
            break;

        case odin_vertex_element_format_vec3f:
            *element_size = sizeof(float[3]);
            *format = VK_FORMAT_R32G32B32_SFLOAT;
            break;

        case odin_vertex_element_format_vec3i32:
            *element_size = sizeof(int32_t[3]);
            *format = VK_FORMAT_R32G32B32_SINT;
            break;

        case odin_vertex_element_format_vec4f:
            *element_size = sizeof(float[4]);
            *format = VK_FORMAT_R32G32B32A32_SFLOAT;
            break;
        
        case odin_vertex_element_format_vec4i32:
            *element_size = sizeof(int32_t[4]);
            *format = VK_FORMAT_R32G32B32A32_SINT;
            break;
        
    };
}


/** \brief Adds an element to a vertex assembly for description. */
void odin_vulkan_vertex_assembly_describe_element(odin_render_device render_device, odin_vertex_assembly vertex_assembly, int element_index, odin_vertex_element_format format)
{

    /* Get the vulkan vertex assembly. */
    odin_vulkan_vertex_assembly vulkan_vertex_assembly = (odin_vulkan_vertex_assembly)vertex_assembly;


    /* Get the element size */
    uint32_t element_size = 0;
    VkFormat element_format = 0;

    odin_vulkan_vertex_format_to_vulkan(format, &element_size, &element_format);

    /* Get the element offset. */
    uint32_t offset = 0;
    
    if(element_index > 0)
    {
        uint32_t previous_element_size = 0;
        VkFormat previous_element_format = 0;

        odin_vulkan_vertex_format_to_vulkan(previous_element_size, &previous_element_size, &previous_element_format);
    
        offset = vulkan_vertex_assembly->attributes[element_index - 1].offset + previous_element_size;
    }


    vulkan_vertex_assembly->attributes[element_index].binding = vulkan_vertex_assembly->binding_description.binding;
    vulkan_vertex_assembly->attributes[element_index].format = element_format;
    vulkan_vertex_assembly->attributes[element_index].location = element_index;
    vulkan_vertex_assembly->attributes[element_index].offset = offset;

}
