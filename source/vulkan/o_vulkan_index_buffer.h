#ifndef __ODIN_VULKAN_BUFFER_INDEX_H__
#define __ODIN_VULKAN_BUFFER_INDEX_H__


#include "../o_index_buffer.h"

#include <vulkan/vulkan.h>
#include "vma.h"


/* A buffer with vertex indices */
typedef struct odin_vulkan_index_buffer
{
    VkBuffer buffer;
    VmaAllocation allocation;
} odin_vulkan_index_buffer;

typedef struct odin_vulkan_index_buffer_dynamic
{
    int TODO;
} odin_vulkan_index_buffer_dynamic;


/* Creates a buffer on the GPU with vertex indices */
odin_index_buffer *odin_vulkan_index_buffer_create
(odin_data *data, uint32_t *indices, int32_t index_count); 

/* Destroys a index buffer */
void odin_vulkan_index_buffer_destroy
(odin_data *data, odin_index_buffer *index_buffer);

/* Creates a dynamic buffer on the GPU with vertex indices */
odin_index_buffer_dynamic *odin_vulkan_index_buffer_dynamic_create
(odin_data *data, uint32_t *indices, int32_t index_count); 

/* Destroys a dynamic index buffer */
void odin_vulkan_index_buffer_dynamic_destroy
(odin_data *data, odin_index_buffer_dynamic *index_buffer_dynamic); 

/* Changes the data in a dynamic index buffer */
void odin_vulkan_index_buffer_dynamic_update
(odin_data *data, odin_index_buffer_dynamic *index_buffer_dynamic, uint32_t *indices, int32_t index_count); 


#endif /* __ODIN_VULKAN_BUFFER_INDEX_H__ */