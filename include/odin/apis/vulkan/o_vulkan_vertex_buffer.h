#ifndef __ODIN_VULKAN_VERTEX_BUFFER_H__
#define __ODIN_VULKAN_VERTEX_BUFFER_H__


#include "odin/o_vertex_buffer.h"

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"


typedef struct odin_vulkan_vertex_buffer_t
{

    VkBuffer buffer;
    VmaAllocation allocation;

} odin_vulkan_vertex_buffer_t, *odin_vulkan_vertex_buffer;


/** \brief Creates a Vulkan vertex buffer. */
void odin_vulkan_vertex_buffer_create(odin_render_device render_device, odin_vertex_buffer* vertex_buffer, odin_vertex_assembly vertex_assembly, int vertices_count, void* vertex_data);

/** \brief Destroys a Vulkan vertex buffer from the gpu. */
void odin_vulkan_vertex_buffer_destroy(odin_render_device render_device, odin_vertex_buffer vertex_buffer);


#endif /* __ODIN_VULKAN_VERTEX_BUFFER_H__ */