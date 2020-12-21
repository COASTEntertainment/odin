#include "odin/apis/vulkan/o_vulkan_index_buffer.h"

#include "aero/a_memory.h"

#include "odin/o_log.h"
#include "odin/apis/vulkan/o_vulkan_render_device.h"


void odin_vulkan_index_buffer_create(odin_render_device render_device, odin_index_buffer* index_buffer, int indices_count, uint32_t* indices_data)
{

        /* Get the vulkan render device. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;


    /* Allocate the vulkan vertex buffer object. */
    odin_vulkan_index_buffer vulkan_index_buffer = NEW(odin_vulkan_index_buffer_t, 1);
    *index_buffer = (odin_index_buffer)vulkan_index_buffer;    


    /* Create the staging buffer. */
    VkBufferCreateInfo staging_buffer_create_info = { 0 };
    staging_buffer_create_info.sType                     = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    staging_buffer_create_info.pNext                     = NULL;
    staging_buffer_create_info.flags                     = 0;
    staging_buffer_create_info.size                      = sizeof(uint32_t) * indices_count;
    staging_buffer_create_info.usage                     = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    staging_buffer_create_info.sharingMode               = VK_SHARING_MODE_EXCLUSIVE;
    staging_buffer_create_info.queueFamilyIndexCount     = 0;
    staging_buffer_create_info.pQueueFamilyIndices       = NULL;

    VmaAllocationCreateInfo staging_allocation_create_info = { 0 };
    staging_allocation_create_info.flags             = 0;
    staging_allocation_create_info.usage             = VMA_MEMORY_USAGE_CPU_ONLY;
    staging_allocation_create_info.requiredFlags     = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    staging_allocation_create_info.preferredFlags    = 0;
    staging_allocation_create_info.memoryTypeBits    = 0;
    staging_allocation_create_info.pool              = NULL;
    staging_allocation_create_info.pUserData         = NULL;

    VkBuffer staging_buffer             = VK_NULL_HANDLE;
    VmaAllocation staging_allocation    = VK_NULL_HANDLE;

    VmaAllocationInfo staging_allocation_info = { 0 };

    vmaCreateBuffer(vulkan_render_device->memory_allocator, &staging_buffer_create_info, &staging_allocation_create_info, &staging_buffer, &staging_allocation, &staging_allocation_info);


    /* Create the actual index buffer. */
    VkBufferCreateInfo buffer_create_info = { 0 };
    buffer_create_info.sType                     = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.pNext                     = NULL;
    buffer_create_info.flags                     = 0;
    buffer_create_info.size                      = sizeof(uint32_t) * indices_count;
    buffer_create_info.usage                     = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    buffer_create_info.sharingMode               = VK_SHARING_MODE_EXCLUSIVE;
    buffer_create_info.queueFamilyIndexCount     = 0;
    buffer_create_info.pQueueFamilyIndices       = NULL;

    VmaAllocationCreateInfo allocation_create_info = { 0 };
    allocation_create_info.flags             = 0;
    allocation_create_info.usage             = VMA_MEMORY_USAGE_GPU_ONLY;
    allocation_create_info.requiredFlags     = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    allocation_create_info.preferredFlags    = 0;
    allocation_create_info.memoryTypeBits    = 0;
    allocation_create_info.pool              = NULL;
    allocation_create_info.pUserData         = NULL;

    VmaAllocationInfo allocation_info = { 0 };

    if (vmaCreateBuffer(vulkan_render_device->memory_allocator, &buffer_create_info, &allocation_create_info, &vulkan_index_buffer->buffer, &vulkan_index_buffer->allocation, &allocation_info) != VK_SUCCESS)
    {
        ODIN_ERROR("o_vulkan_index_buffer.c", "Could not create a vulkan index buffer!");
    }


    /* Transfer the data to the staging buffer. */
    void* data;
    vmaMapMemory(vulkan_render_device->memory_allocator, staging_allocation, &data);
        aero_memcpy(data, sizeof(uint32_t) * indices_count, indices_data, sizeof(uint32_t) * indices_count);
    vmaUnmapMemory(vulkan_render_device->memory_allocator, staging_allocation);


    /* Copy the staging buffer to the index buffer. */
    VkCommandBuffer temporary_command_buffer = VK_NULL_HANDLE;

    VkCommandBufferAllocateInfo command_buffer_allocate_info = { 0 };
    command_buffer_allocate_info.sType                  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.pNext                  = NULL;
    command_buffer_allocate_info.commandPool            = vulkan_render_device->command_pool;
    command_buffer_allocate_info.level                  = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount     = 1;


    vkAllocateCommandBuffers(vulkan_render_device->device, &command_buffer_allocate_info, &temporary_command_buffer);

    VkCommandBufferBeginInfo command_buffer_begin_info = { 0 };
    command_buffer_begin_info.sType     = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext     = NULL;
    command_buffer_begin_info.flags     = 0;
    command_buffer_begin_info.pInheritanceInfo = NULL;

    vkBeginCommandBuffer(temporary_command_buffer, &command_buffer_begin_info);

        VkBufferCopy copy_region = { 0 };
        copy_region.srcOffset   = 0;
        copy_region.dstOffset   = 0;
        copy_region.size        = sizeof(uint32_t) * indices_count;

        vkCmdCopyBuffer(temporary_command_buffer, staging_buffer, vulkan_index_buffer->buffer, 1, &copy_region);

    vkEndCommandBuffer(temporary_command_buffer);

    VkSubmitInfo submit_info = { 0 };
    submit_info.sType                   = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext                   = NULL;
    submit_info.waitSemaphoreCount      = 0;
    submit_info.pWaitSemaphores         = NULL;
    submit_info.pWaitDstStageMask       = NULL;
    submit_info.commandBufferCount      = 1;
    submit_info.pCommandBuffers         = &temporary_command_buffer;
    submit_info.signalSemaphoreCount    = 0;
    submit_info.pSignalSemaphores       = NULL;

    if(vkQueueSubmit(vulkan_render_device->graphics_queue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        ODIN_ERROR("o_vulkan_index_buffer.c", "Could not copy a staging index buffer to the GPU!");
    }

    /* Wait for the queue to finish. */
    vkQueueWaitIdle(vulkan_render_device->graphics_queue);

    vkFreeCommandBuffers(vulkan_render_device->device, vulkan_render_device->command_pool, 1, &temporary_command_buffer);

    /* Destroy the staging buffer. */
    vmaDestroyBuffer(vulkan_render_device->memory_allocator, staging_buffer, staging_allocation);

}

void odin_vulkan_index_buffer_destroy(odin_render_device render_device, odin_index_buffer index_buffer)
{

    /* Get the vulkan render device. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    /* Get the vulkan index buffer. */
    odin_vulkan_index_buffer vulkan_index_buffer = (odin_vulkan_index_buffer)index_buffer;


    /* Destroy the buffer. */
    vmaDestroyBuffer(vulkan_render_device->memory_allocator, vulkan_index_buffer->buffer, vulkan_index_buffer->allocation);

    DELETE(index_buffer);

}