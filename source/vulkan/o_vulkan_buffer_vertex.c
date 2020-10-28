#include "o_vulkan_buffer_vertex.h"

#include "Aero/a_memory.h"

#include "o_vulkan.h"
#include "o_vulkan_buffer.h"


/* Creates a buffer on the GPU with vertex indices */
odin_buffer_vertex* odin_vulkan_buffer_vertex_create
(odin_data* data, odin_vertex_assembly* vertex_assembly, void* vertices, int32_t vertex_count)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Get the vertex assembly */
    odin_vulkan_vertex_assembly* vulkan_vertex_assembly = (odin_vulkan_vertex_assembly*)vertex_assembly;

    /* Get the actual buffer size */
    VkDeviceSize vertex_buffer_size = vulkan_vertex_assembly->input_binding_description.stride * vertex_count;

    /* Create the staging buffer */
    VkBuffer vertex_staging_buffer = VK_NULL_HANDLE;
    VmaAllocation vertex_staging_buffer_allocation = VK_NULL_HANDLE;

    odin_vulkan_specific_buffer_create(vulkan_data, vertex_buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_COPY, 0, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &vertex_staging_buffer, &vertex_staging_buffer_allocation, NULL);

    /* Set the staging buffer memory */
    void* memory_data;
    vmaMapMemory(vulkan_data->memory_allocator, vertex_staging_buffer_allocation, &memory_data);
        memcpy(memory_data, vertices, vertex_buffer_size);
    vmaUnmapMemory(vulkan_data->memory_allocator, vertex_staging_buffer_allocation);

    /* Create the vertex buffer */
    VkBuffer vertex_buffer;
    VmaAllocation vertex_buffer_allocation;

    odin_vulkan_specific_buffer_create(vulkan_data, vertex_buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_GPU_ONLY, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &vertex_buffer, &vertex_buffer_allocation, NULL);

    /* Copy the staging buffer to the GPU buffer */
    odin_vulkan_specific_buffer_copy(vulkan_data, vertex_staging_buffer, vertex_buffer, vertex_buffer_size);

    /* Destroy the staging buffer */
    vmaDestroyBuffer(vulkan_data->memory_allocator, vertex_staging_buffer, vertex_staging_buffer_allocation);

    /* Create the odin_vulkan_buffer_vertex */
    odin_vulkan_buffer_vertex* new_vertex_buffer = (odin_vulkan_buffer_vertex*)aero_malloc(sizeof(odin_vulkan_buffer_vertex));
    new_vertex_buffer->buffer = vertex_buffer;
    new_vertex_buffer->allocation = vertex_buffer_allocation;

    return (odin_buffer_vertex*)new_vertex_buffer;
}

/* Destroys a vertex buffer */
void odin_vulkan_buffer_vertex_destroy
(odin_data* data, odin_buffer_vertex* vertex_buffer)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Get the vulkan buffer */
    odin_vulkan_buffer_vertex* vulkan_vertex_buffer = (odin_vulkan_buffer_vertex*)vertex_buffer;

    /* Destroy the buffer */
    vmaDestroyBuffer(vulkan_data->memory_allocator, vulkan_vertex_buffer->buffer, vulkan_vertex_buffer->allocation);
}

/* Creates a dynamic buffer on the gpu with the specified vertices */
odin_buffer_dynamic_vertex* odin_vulkan_buffer_dynamic_vertex_create
(odin_data* data, odin_vertex_assembly* vertex_assembly, void* vertices, int32_t vertex_count)
{
    return NULL;
}

/* Destroys a dynamic vertex buffer */
void odin_vulkan_buffer_dynamic_vertex_destroy
(odin_data* data, odin_buffer_dynamic_vertex *vertexBufferDynamic)
{

}

/* Changes the data in a dynamic vertex buffer */
void odin_vulkan_buffer_dynamic_vertex_update
(odin_data* data, odin_buffer_dynamic_vertex* vertexBufferDynamic, void* vertices, int32_t vertexCount)
{

}

    
