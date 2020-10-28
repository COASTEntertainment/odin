#include "o_vulkan_vertex_assembly.h"


odin_vertex_assembly* odin_vulkan_vertex_assembly_create
(odin_data* data, uint32_t vertex_size)
{
    /* Check and make sure that odin_vulkan_vertex_assemblys is initalized */
    if(odin_vulkan_vertex_assemblys.count == 0)
        dynarr_init(&odin_vulkan_vertex_assemblys, sizeof(odin_vulkan_vertex_assembly*));

    /* Allocate the vertex assembly */
    odin_vulkan_vertex_assembly* vulkan_vertex_assembly = (odin_vulkan_vertex_assembly*)aero_malloc(sizeof(odin_vulkan_vertex_assembly));

    /* Set the input_binding_description */
    vulkan_vertex_assembly->input_binding_description.binding = odin_vulkan_vertex_assemblys.count;
    vulkan_vertex_assembly->input_binding_description.stride = (uint32_t)vertex_size;
    vulkan_vertex_assembly->input_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    /* Initalize the input_attribute_descriptions */
    dynarr_init(&vulkan_vertex_assembly->input_attribute_descriptions, sizeof(VkVertexInputAttributeDescription));

    /* Add this new vulkan_vertex_assembly to the odin_vulkan_vertex_assemblys */
    dynarr_add(&odin_vulkan_vertex_assemblys, vulkan_vertex_assembly);

    return (odin_vertex_assembly*)vulkan_vertex_assembly;
}

void odin_vulkan_vertex_assembly_destroy
(odin_data* data, odin_vertex_assembly* vertex_assembly)
{
    /* Convert to odin_vulkan_vertex_assembly */
    odin_vulkan_vertex_assembly* vulkan_vertex_assembly = (odin_vulkan_vertex_assembly*)vertex_assembly;

    /* free the elements array */
    dynarr_free(&vulkan_vertex_assembly->input_attribute_descriptions);

    /* Free the vulkan_vertex_assembly */
    aero_free(vulkan_vertex_assembly);
}

void odin_vulkan_vertex_assembly_add_element
(odin_data* data, odin_vertex_assembly* vertex_assembly, odin_vertex_element_type elementType, aero_size_t offset)
{
    /* Convert to a vulkan_vertex_assembly */
    odin_vulkan_vertex_assembly* vulkan_vertex_assembly = (odin_vulkan_vertex_assembly*)vertex_assembly;

    /* Vertex assembly element to vulkan format */
    VkFormat element_format;

    switch (elementType)
    {

        case odin_vertex_element_type_int:
            element_format = VK_FORMAT_R32_SINT;
        break;

        case odin_vertex_element_type_unsigned_int:
            element_format = VK_FORMAT_R32_UINT;
        break;
    
        case odin_vertex_element_type_normalized_int:
            element_format = VK_FORMAT_R8_SNORM;
        break;

        case odin_vertex_element_type_float:
            element_format = VK_FORMAT_R32_SFLOAT;
        break;

        case odin_vertex_element_type_vec2i:
            element_format = VK_FORMAT_R32G32_SINT;
        break;

        case odin_vertex_element_type_vec2f:
            element_format = VK_FORMAT_R32G32_SFLOAT;
        break;

        case odin_vertex_element_type_vec3i:
            element_format = VK_FORMAT_R32G32B32_SINT;
        break;

        case odin_vertex_element_type_vec3f:
            element_format = VK_FORMAT_R32G32B32_SFLOAT;
        break;

        case odin_vertex_element_type_vec4i:
            element_format = VK_FORMAT_R32G32B32A32_SINT;
        break;

        case odin_vertex_element_type_vec4f:
            element_format = VK_FORMAT_R32G32B32A32_SFLOAT;
        break;


    default:
        break;
    }

    VkVertexInputAttributeDescription* vertex_attribute = (VkVertexInputAttributeDescription*)aero_malloc(sizeof(VkVertexInputAttributeDescription*));
    vertex_attribute->location = (uint32_t)vulkan_vertex_assembly->input_attribute_descriptions.count;
    vertex_attribute->binding = vulkan_vertex_assembly->input_binding_description.binding;
    vertex_attribute->format = element_format;
    vertex_attribute->offset = (uint32_t)offset;

    dynarr_add(&vulkan_vertex_assembly->input_attribute_descriptions, vertex_attribute);

}