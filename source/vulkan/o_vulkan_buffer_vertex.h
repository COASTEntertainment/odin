#ifndef __ODIN_VULKAN_BUFFER_VERTEX_H__
#define __ODIN_VULKAN_BUFFER_VERTEX_H__


#include "../o_buffer_vertex.h"
#include "o_vulkan_vertex_assembly.h"

#include <vulkan/vulkan.h>

#include "vma.h"


/* A buffer with vertices */
typedef struct odin_vulkan_buffer_vertex
{
    VkBuffer buffer;
    VmaAllocation allocation;
} odin_vulkan_buffer_vertex;

/* A dynamic buffer with vertices */
typedef struct odin_vulkan_buffer_dynamic_vertex 
{
    int TODO;
} odin_vulkan_buffer_dynamic_vertex; 


/* Creates a buffer on the gpu with the specified vertices */
odin_buffer_vertex* odin_vulkan_buffer_vertex_create
(odin_data* data, odin_vertex_assembly* vertexAssembly, void* vertices, int32_t vertex_count); 

/* Destroys a vertex buffer */
void odin_vulkan_buffer_vertex_destroy
(odin_data* data, odin_buffer_vertex* vertexBuffer); 

/* Creates a dynamic buffer on the gpu with the specified vertices */
odin_buffer_dynamic_vertex* odin_vulkan_buffer_dynamic_vertex_create
(odin_data* data, odin_vertex_assembly* vertexAssembly, void* vertices, int32_t vertex_count); 

/* Destroys a dynamic vertex buffer */
void odin_vulkan_buffer_dynamic_vertex_destroy
(odin_data* data, odin_buffer_dynamic_vertex *vertexBufferDynamic);

/* Changes the data in a dynamic vertex buffer */
void odin_vulkan_buffer_dynamic_vertex_update
(odin_data* data, odin_buffer_dynamic_vertex* vertexBufferDynamic, void* vertices, int32_t vertexCount); 


#endif /* __ODIN_VULKAN_BUFFER_VERTEX_H__ */