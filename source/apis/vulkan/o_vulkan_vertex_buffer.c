#include "odin/apis/vulkan/o_vulkan_vertex_buffer.h"

#include "aero/a_memory.h"

#include "odin/o_log.h"
#include "odin/apis/vulkan/o_vulkan_render_device.h"
#include "odin/apis/vulkan/vk_mem_alloc.h"
#include "odin/apis/vulkan/o_vulkan_vertex_assembly.h"


void odin_vulkan_vertex_buffer_create(odin_render_device render_device, odin_vertex_buffer* vertex_buffer, odin_vertex_assembly vertex_assembly, int vertices_count, void* vertex_data)
{

    /* Get the vulkan render device. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    /* Get the vulkan vertex assembly. */
    odin_vulkan_vertex_assembly vulkan_vertex_assembly = (odin_vulkan_vertex_assembly)vertex_assembly;

    /* Allocate the vulkan vertex buffer object. */
    odin_vulkan_vertex_buffer vulkan_vertex_buffer = malloc(sizeof(odin_vulkan_vertex_buffer_t));
    *vertex_buffer = (odin_vertex_buffer)vulkan_vertex_buffer;    


    /* Create the staging buffer. */
    VkBufferCreateInfo staging_buffer_create_info = { 0 };
    staging_buffer_create_info.sType                     = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    staging_buffer_create_info.pNext                     = NULL;
    staging_buffer_create_info.flags                     = 0;
    staging_buffer_create_info.size                      = vulkan_vertex_assembly->binding_description.stride * vertices_count;
    staging_buffer_create_info.usage                     = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    staging_buffer_create_info.sharingMode               = VK_SHARING_MODE_EXCLUSIVE;
    staging_buffer_create_info.queueFamilyIndexCount     = 0;
    staging_buffer_create_info.pQueueFamilyIndices       = NULL;

    VmaAllocationCreateInfo staging_allocation_create_info = { 0 };
    staging_allocation_create_info.flags             = 0;
    staging_allocation_create_info.usage             = VMA_MEMORY_USAGE_CPU_ONLY;
    staging_allocation_create_info.requiredFlags     = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    staging_allocation_create_info.preferredFlags    = 0;
    staging_allocation_create_info.memoryTypeBits    = NULL;
    staging_allocation_create_info.pool              = NULL;
    staging_allocation_create_info.pUserData         = NULL;

    VkBuffer staging_buffer             = VK_NULL_HANDLE;
    VmaAllocation staging_allocation    = VK_NULL_HANDLE;

    VmaAllocationInfo staging_allocation_info = { 0 };

    vmaCreateBuffer(vulkan_render_device->memory_allocator, &staging_buffer_create_info, &staging_allocation_create_info, &staging_buffer, &staging_allocation, &staging_allocation_info);


    /* Create the actual vertex buffer. */
    VkBufferCreateInfo buffer_create_info = { 0 };
    buffer_create_info.sType                     = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.pNext                     = NULL;
    buffer_create_info.flags                     = 0;
    buffer_create_info.size                      = vulkan_vertex_assembly->binding_description.stride * vertices_count;
    buffer_create_info.usage                     = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buffer_create_info.sharingMode               = VK_SHARING_MODE_EXCLUSIVE;
    buffer_create_info.queueFamilyIndexCount     = 0;
    buffer_create_info.pQueueFamilyIndices       = NULL;

    VmaAllocationCreateInfo allocation_create_info = { 0 };
    allocation_create_info.flags             = 0;
    allocation_create_info.usage             = VMA_MEMORY_USAGE_GPU_ONLY;
    allocation_create_info.requiredFlags     = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    allocation_create_info.preferredFlags    = 0;
    allocation_create_info.memoryTypeBits    = NULL;
    allocation_create_info.pool              = NULL;
    allocation_create_info.pUserData         = NULL;

    VmaAllocationInfo allocation_info = { 0 };

    vmaCreateBuffer(vulkan_render_device->memory_allocator, &buffer_create_info, &allocation_create_info, &vulkan_vertex_buffer->buffer, &vulkan_vertex_buffer->allocation, &allocation_info);


    /* Transfer the data to the staging buffer. */
    void* data;
    vmaMapMemory(vulkan_render_device->memory_allocator, staging_allocation, &data);
        aero_memcpy(data, vulkan_vertex_assembly->binding_description.stride * vertices_count, vertex_data, vulkan_vertex_assembly->binding_description.stride * vertices_count);
    vmaUnmapMemory(vulkan_render_device->memory_allocator, staging_allocation);


    /* Copy the staging buffer to the vertex buffer. */
    VkCommandBuffer temporary_command_buffer = VK_NULL_HANDLE;

    VkCommandBufferBeginInfo command_buffer_begin_info = { 0 };
    command_buffer_begin_info.sType     = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext     = NULL;
    command_buffer_begin_info.flags     = 0;
    command_buffer_begin_info.pInheritanceInfo = NULL;

    vkBeginCommandBuffer(temporary_command_buffer, &command_buffer_begin_info);

        VkBufferCopy copy_region = { 0 };
        copy_region.srcOffset   = staging_allocation_info.offset;
        copy_region.dstOffset   = allocation_info.offset;
        copy_region.size        = allocation_info.size;

        vkCmdCopyBuffer(temporary_command_buffer, staging_buffer, vulkan_vertex_buffer->buffer, 1, &copy_region);

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
        ODIN_ERROR("o_vulkan_vertex_buffer.c", "Could not copy a staging vertex buffer to the GPU!");
    }

    /* Destroy the staging buffer. */
    vmaDestroyBuffer(vulkan_render_device->memory_allocator, staging_buffer, staging_allocation);

}

void odin_vulkan_vertex_buffer_destroy(odin_render_device render_device, odin_vertex_buffer vertex_buffer)
{

    /* Get the vulkan render device. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    /* Get the vulkan vertex buffer. */
    odin_vulkan_vertex_buffer vulkan_vertex_buffer = (odin_vulkan_vertex_buffer)vertex_buffer;


    /* Destroy the buffer. */
    vmaDestroyBuffer(vulkan_render_device->memory_allocator, vulkan_vertex_buffer->buffer, vulkan_vertex_buffer->allocation);

}