#include "o_vulkan_buffer.h"


#include "o_vulkan.h"
#include "vma.h"


/* Create a vulkan buffer */
void odin_vulkan_specific_buffer_create
(odin_vulkan_data* vulkan_data, VkDeviceSize buffer_size, VkBufferUsageFlags buffer_usage, VmaMemoryUsage memory_usage, VmaAllocationCreateFlags allocation_flags, VkMemoryPropertyFlags properties, VkBuffer* buffer, VmaAllocation* buffer_allocation, VmaAllocationInfo* allocation_info)
{
    /* Build the buffer info */
    VkBufferCreateInfo buffer_create_info = { 0 };
    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.pNext = NULL;
    buffer_create_info.flags = 0;
    buffer_create_info.size = buffer_size;
    buffer_create_info.usage = buffer_usage;
    buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buffer_create_info.queueFamilyIndexCount = 0;
    buffer_create_info.pQueueFamilyIndices = NULL;

    /* Build the vma allocation info */
    VmaAllocationCreateInfo allocation_create_info = { 0 };
    allocation_create_info.flags = 0;
    allocation_create_info.usage = memory_usage;
    allocation_create_info.requiredFlags = properties;
    allocation_create_info.preferredFlags = 0;
    allocation_create_info.memoryTypeBits = VK_NULL_HANDLE;
    allocation_create_info.pool = VK_NULL_HANDLE;
    allocation_create_info.pUserData = NULL;

    /* Check and make sure that the buffer has been created */
    VK_CHECK(vmaCreateBuffer(vulkan_data->memory_allocator, &buffer_create_info, &allocation_create_info, buffer, buffer_allocation, allocation_info), "Could not allocate a buffer");
}

/* Copys a vulkan buffer to another */
void odin_vulkan_specific_buffer_copy
(odin_vulkan_data* vulkan_data, VkBuffer source_buffer, VkBuffer destination_buffer, VkDeviceSize size)
{

    /* Create the single time command buffer */
    VkCommandBuffer command_buffer = odin_vulkan_specific_begin_single_time_commands(vulkan_data);

    /* Build the copy region */
    VkBufferCopy copy_region = { 0 };
    copy_region.srcOffset = 0;
    copy_region.dstOffset = 0;
    copy_region.size = size;

    /* Do the copy command */
    vkCmdCopyBuffer(command_buffer, source_buffer, destination_buffer, 1, &copy_region);

    /* End the single time commands */
    odin_vulkan_specific_end_single_time_commands(vulkan_data, command_buffer);


}