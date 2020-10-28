#ifndef __ODIN_VULKAN_VERTEX_ASSEMBLY_H__
#define __ODIN_VULKAN_VERTEX_ASSEMBLY_H__


#include <stdint.h>

#include "../o_vertex_assembly.h"

#include <vulkan/vulkan.h>


/* Different types of information that can be passed with a vertex element */
typedef struct odin_vulkan_vertex_assembly
{

    VkVertexInputBindingDescription     input_binding_description;
    VkVertexInputAttributeDescription*  input_attribute_descriptions;

} odin_vulkan_vertex_assembly;


/* Creates a vertex assembly */
extern odin_vertex_assembly* odin_vulkan_vertex_assembly_create
(odin_data *data, int32_t vertex_size);

/* Destroys a vertex assembly because it will not be used anymore */
extern void odin_vulkan_vertex_assembly_destroy
(odin_data *data, odin_vertex_assembly *vertex_assembly); 

 /* Adds an element to a vertex assembly */
extern void odin_vulkan_vertex_assembly_add_element
(odin_data *data, odin_vertex_assembly *vertex_assembly, odin_vertex_element_type element_type, int32_t offset);


#endif /* __ODIN_VULKAN_VERTEX_ASSEMBLY_H__ */