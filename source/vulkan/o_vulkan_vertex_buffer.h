#ifndef __ODIN_VULKAN_BUFFER_VERTEX_H__
#define __ODIN_VULKAN_BUFFER_VERTEX_H__


#include "../o_vertex_buffer.h"
#include "o_vulkan_vertex_assembly.h"

#include <vulkan/vulkan.h>

#include "vma.h"


/* A buffer with vertices */
typedef struct odin_vulkan_vertex_buffer
{
    VkBuffer buffer;
    VmaAllocation allocation;
} odin_vulkan_vertex_buffer;

/* A dynamic buffer with vertices */
typedef struct odin_vulkan_vertex_buffer_dynamic 
{
    int TODO;
} odin_vulkan_vertex_buffer_dynamic; 


/* Creates a buffer on the gpu with the specified vertices */
odin_vertex_buffer *odin_vulkan_vertex_buffer_create
(odin_data *data, odin_vertex_assembly *vertex_assembly, void *vertices, int32_t vertex_count); 

/* Destroys a vertex buffer */
void odin_vulkan_vertex_buffer_destroy
(odin_data *data, odin_vertex_buffer *vertex_buffer); 

/* Creates a dynamic buffer on the gpu with the specified vertices */
odin_vertex_buffer_dynamic *odin_vulkan_vertex_buffer_dynamic_create
(odin_data *data, odin_vertex_assembly *vertex_assembly, void *vertices, int32_t vertex_count); 

/* Destroys a dynamic vertex buffer */
void odin_vulkan_vertex_buffer_dynamic_destroy
(odin_data *data, odin_vertex_buffer_dynamic *vertex_buffer_dynamic);

/* Changes the data in a dynamic vertex buffer */
void odin_vulkan_vertex_buffer_dynamic_update
(odin_data *data, odin_vertex_buffer_dynamic *vertex_buffer_dynamic, void *vertices, int32_t vertex_count); 


#endif /* __ODIN_VULKAN_BUFFER_VERTEX_H__ */