#include "o_vulkan_buffer_index.h"


#include "o_vulkan.h"
#include "o_vulkan_buffer.h"

#include "Aero/a_memory.h"


/* Creates a buffer on the GPU with index indices */
odin_buffer_index* odin_vulkan_buffer_index_create
(odin_data* data, uint32_t* indices, int32_t index_count)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Get the actual buffer size */
    VkDeviceSize index_buffer_size = sizeof(uint32_t) * index_count;

    // Create index buffer
    VkBuffer index_buffer;
    VmaAllocation index_buffer_allocation;

    VkBufferCreateInfo ibInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    ibInfo.size = index_buffer_size;
    ibInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    ibInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    VmaAllocationCreateInfo ibAllocCreateInfo = { 0 };
    ibAllocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
    ibAllocCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
    
    VkBuffer stagingIndexBuffer = VK_NULL_HANDLE;
    VmaAllocation stagingIndexBufferAlloc = VK_NULL_HANDLE;
    VmaAllocationInfo stagingIndexBufferAllocInfo = { 0 };
    VK_CHECK(vmaCreateBuffer(vulkan_data->memory_allocator, &ibInfo, &ibAllocCreateInfo, &stagingIndexBuffer, &stagingIndexBufferAlloc, &stagingIndexBufferAllocInfo), "");

    memcpy(stagingIndexBufferAllocInfo.pMappedData, indices, index_buffer_size);

    // No need to flush stagingIndexBuffer memory because CPU_ONLY memory is always HOST_COHERENT.

    ibInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    ibAllocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    ibAllocCreateInfo.flags = 0;
    VK_CHECK(vmaCreateBuffer(vulkan_data->memory_allocator, &ibInfo, &ibAllocCreateInfo, &index_buffer, &index_buffer_allocation, NULL), "");

    // Copy buffers

    VkCommandBuffer command_buffer = odin_vulkan_specific_begin_single_time_commands(vulkan_data);

        VkBufferCopy ibCopyRegion = { 0 };
        ibCopyRegion.srcOffset = 0;
        ibCopyRegion.dstOffset = 0;
        ibCopyRegion.size = ibInfo.size;
        vkCmdCopyBuffer(command_buffer, stagingIndexBuffer, index_buffer, 1, &ibCopyRegion);

    odin_vulkan_specific_end_single_time_commands(vulkan_data, command_buffer);

    vmaDestroyBuffer(vulkan_data->memory_allocator, stagingIndexBuffer, stagingIndexBufferAlloc);


    /* Create the odin_vulkan_buffer_index */
    odin_vulkan_buffer_index* new_index_buffer = (odin_vulkan_buffer_index*)aero_malloc(sizeof(odin_vulkan_buffer_index));
    new_index_buffer->buffer = index_buffer;
    new_index_buffer->allocation = index_buffer_allocation;

    return (odin_buffer_index*)new_index_buffer;
}

/* Destroys a index buffer */
void odin_vulkan_buffer_index_destroy
(odin_data* data, odin_buffer_index* index_buffer)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Get the odin_vulkan_buffer_index */
    odin_vulkan_buffer_index* vulkan_index_buffer = (odin_vulkan_buffer_index*)index_buffer;

    /* Destroy the buffer */
    vmaDestroyBuffer(vulkan_data->memory_allocator, vulkan_index_buffer->buffer, vulkan_index_buffer->allocation);
}

/* Creates a dynamic buffer on the GPU with index indices */
odin_buffer_dynamic_index* odin_vulkan_buffer_dynamic_index_create
(odin_data* data, uint32_t* indices, int32_t indexCount)
{
    return NULL;
}

/* Destroys a dynamic index buffer */
void odin_vulkan_buffer_dynamic_index_destroy
(odin_data* data, odin_buffer_dynamic_index* indexBufferDynamic)
{

}

/* Changes the data in a dynamic index buffer */
void odin_vulkan_buffer_dynamic_index_update
(odin_data* data, odin_buffer_dynamic_index* indexBufferDynamic, uint32_t* indices, int32_t indexCount)
{

}