#ifndef __ODIN_VULKAN_VERTEX_ASSEMBLY_H__
#define __ODIN_VULKAN_VERTEX_ASSEMBLY_H__


#include "odin/o_render_device.h"
#include "odin/o_vertex_assembly.h"


#include <vulkan/vulkan.h>


/** \brief The vulkan abstraction of a vertex assembly. */
typedef struct odin_vulkan_vertex_assembly_t
{

    VkVertexInputBindingDescription binding_description; /** \brief The vulkan definition of a vertex assembly size and index. */
    VkVertexInputAttributeDescription *attributes; /** \brief The vertex attributes in vulkan. */
    uint32_t attributes_count; /** \brief Indicates how many attributes are in the attributes pointer. */

} odin_vulkan_vertex_assembly_t, *odin_vulkan_vertex_assembly;


/** \brief Creates a vertex assembly. */
void odin_vulkan_vertex_assembly_create(odin_render_device render_device, odin_vertex_assembly* vertex_assembly, int binding, int32_t vertex_size, int elements_count);

/** \brief Destroys a vertex assembly. */
void odin_vulkan_vertex_assembly_destroy(odin_render_device render_device, odin_vertex_assembly vertex_assembly);

/** \brief Adds an element to a vertex assembly for description. */
void odin_vulkan_vertex_assembly_describe_element(odin_render_device render_device, odin_vertex_assembly vertex_assembly, int element_index, odin_vertex_element_format format);



#endif /* __ODIN_VULKAN_VERTEX_ASSEMBLY_H__ */