#include "o_vulkan_buffer_command.h"


#include "o_vulkan.h"


/* Single time commands for any task that can run in multiple threads */
VkCommandBuffer odin_vulkan_specific_begin_single_time_commands
(odin_vulkan_data* data)
{
    /* Build the command buffer allocate info */
    VkCommandBufferAllocateInfo allocate_info = { 0 };
    allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocate_info.pNext = NULL;
    allocate_info.commandPool = data->command_pool;
    allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocate_info.commandBufferCount = 1;

    /* Create the command buffer */
    VkCommandBuffer command_buffer = VK_NULL_HANDLE;
    VK_CHECK(vkAllocateCommandBuffers(data->device, &allocate_info, &command_buffer), "Could not create a single time command buffer!");

    /* Build the begin info */
    VkCommandBufferBeginInfo begin_info = { 0 };
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.pNext = NULL;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    begin_info.pInheritanceInfo = NULL;

    /* Begin the command buffer */
    vkBeginCommandBuffer(command_buffer, &begin_info);

    return command_buffer;
}

/* Ends single time commands */
void odin_vulkan_specific_end_single_time_commands
(odin_vulkan_data* vulkan_data, VkCommandBuffer command_buffer)
{
    /* End the command buffer */
    vkEndCommandBuffer(command_buffer);

    /* Submit info */
    VkSubmitInfo submit_info = { 0 };
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = NULL;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    VK_CHECK(vkQueueSubmit(vulkan_data->graphics_queue, 1, &submit_info, VK_NULL_HANDLE), "Could not submit a one time command buffer!");
    vkQueueWaitIdle(vulkan_data->graphics_queue);

}
