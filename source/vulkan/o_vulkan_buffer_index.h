#ifndef __ODIN_VULKAN_BUFFER_INDEX_H__
#define __ODIN_VULKAN_BUFFER_INDEX_H__


#include "../o_buffer_index.h"

#include <vulkan/vulkan.h>
#include "vma.h"


/* A buffer with vertex indices */
typedef struct odin_vulkan_buffer_index
{
    VkBuffer buffer;
    VmaAllocation allocation;
} odin_vulkan_buffer_index;

typedef struct odin_vulkan_buffer_dynamic_index
{
    int TODO;
} odin_vulkan_buffer_dynamic_index;


/* Creates a buffer on the GPU with vertex indices */
odin_buffer_index* odin_vulkan_buffer_index_create
(odin_data* data, uint32_t* indices, int32_t indexCount); 

/* Destroys a index buffer */
void odin_vulkan_buffer_index_destroy
(odin_data* data, odin_buffer_index* indexBuffer);

/* Creates a dynamic buffer on the GPU with vertex indices */
odin_buffer_dynamic_index* odin_vulkan_buffer_dynamic_index_create
(odin_data* data, uint32_t* indices, int32_t indexCount); 

/* Destroys a dynamic index buffer */
void odin_vulkan_buffer_dynamic_index_destroy
(odin_data* data, odin_buffer_dynamic_index* indexBufferDynamic); 

/* Changes the data in a dynamic index buffer */
void odin_vulkan_buffer_dynamic_index_update
(odin_data* data, odin_buffer_dynamic_index* indexBufferDynamic, uint32_t* indices, int32_t indexCount); 


#endif /* __ODIN_VULKAN_BUFFER_INDEX_H__ */