#include "o_vulkan_draw.h"

#include "Aero/a_memory.h"

#include "o_vulkan_data.h"
#include "o_vulkan_platform.h"
#include "o_vulkan.h"


/* When all required render objects are created we can prepare for rendering */
void odin_vulkan_draw_prepare
(odin_data* data)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Build the color attachment description */
    VkAttachmentDescription color_attachment_description = { 0 };
    color_attachment_description.flags = 0;
    color_attachment_description.format = vulkan_data->platform_data->format.format;
    color_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment_description.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    /* Build the position attachment description */

    /* Build the normal attachment description */

    /* Build the depth attachment description */

    VkAttachmentDescription attachment_descriptions[] = { color_attachment_description };


    /* Build the attachment refrence */
    VkAttachmentReference color_attachment_reference = { 0 };
    color_attachment_reference.attachment = 0;
    color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    /* Build the position attachment refrence */

    /* Build the normal attachment refrence */

    /* Build the depth attachment refrence */

    VkAttachmentReference color_attachment_references[] = { color_attachment_reference };


    /* Build the subpass */
    VkSubpassDescription subpass_description = { 0 };
    subpass_description.flags = 0;
    subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass_description.inputAttachmentCount = 0;
    subpass_description.pInputAttachments = NULL;
    subpass_description.colorAttachmentCount = 1;
    subpass_description.pColorAttachments = color_attachment_references;
    subpass_description.pResolveAttachments = NULL;
    subpass_description.pDepthStencilAttachment = NULL;
    subpass_description.preserveAttachmentCount = 0;
    subpass_description.pPreserveAttachments = 0;

    /* Build the renderpass create info */
    VkRenderPassCreateInfo render_pass_create_info = { 0 };
    render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.pNext = NULL;
    render_pass_create_info.flags = 0;
    render_pass_create_info.attachmentCount = 1;
    render_pass_create_info.pAttachments = attachment_descriptions;
    render_pass_create_info.subpassCount = 0;

    /* Create the render pass */
    vkCreateRenderPass(vulkan_data->device, &render_pass_create_info, NULL, &vulkan_data->render_pass);

    /* Build the semaphore sync objects create info */
    VkSemaphoreCreateInfo semaphore_create_info = { 0 };
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = NULL;
    semaphore_create_info.flags = 0;

    /* Create the semaphore sync objects */
    vulkan_data->image_available_semaphores = (VkSemaphore*)aero_malloc(sizeof(VkSemaphore) * vulkan_data->platform_data->swapchain_images_count);
    vulkan_data->render_finished_semaphores = (VkSemaphore*)aero_malloc(sizeof(VkSemaphore) * vulkan_data->platform_data->swapchain_images_count);

    for(uint32_t i = 0; i < vulkan_data->platform_data->swapchain_images_count; i++)
    {
        VK_CHECK(vkCreateSemaphore(vulkan_data->device, &semaphore_create_info, NULL, &vulkan_data->image_available_semaphores[i]), "Could not create the image available semaphore!");
        VK_CHECK(vkCreateSemaphore(vulkan_data->device, &semaphore_create_info, NULL, &vulkan_data->render_finished_semaphores[i]), "Could not create the render finished semaphore!");
    }

    /* Build the fence info */
    VkFenceCreateInfo fence_info = { 0 };
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.pNext = NULL;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    /* Create the fence sync objects */
    vulkan_data->in_flight_fences = (VkFence*)aero_malloc(sizeof(VkFence) * vulkan_data->platform_data->swapchain_images_count);
    vulkan_data->images_in_flight = (VkFence*)aero_malloc(sizeof(VkFence) * vulkan_data->platform_data->swapchain_images_count);

    for(uint32_t i = 0; i < vulkan_data->platform_data->swapchain_images_count; i++)
    {
        VK_CHECK(vkCreateFence(vulkan_data->device, &fence_info, NULL, &vulkan_data->in_flight_fences[i]), "Could not create a fence!");
        vulkan_data->images_in_flight[i] = VK_NULL_HANDLE;
    }

}


/* Changes how rendering happens */
void odin_vulkan_draw_set_function
(odin_data* data, void(* odin_draw_func)(odin_data* data, odin_buffer_command* commandBuffer))
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Set the draw function */
    vulkan_data->odin_current_draw_func = odin_draw_func;
}

/* Sets the clear color */
void odin_vulkan_render_set_clear_color_vec4f        
(odin_data* data, float* color)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    aero_memcpy(vulkan_data->clear_color, sizeof(vec4f), color, sizeof(vec4f));

}

/* Binds a pipeline for rendering */
void odin_vulkan_draw_set_pipeline
(odin_data* data, odin_buffer_command* command_buffer, odin_pipeline* pipeline)
{
    /* Get the vulkan command buffers */
    odin_vulkan_buffer_command* vulkan_command_buffer = (odin_vulkan_buffer_command*)command_buffer;

    /* Get the vulkan pipeline */
    odin_vulkan_pipeline* vulkan_pipeline = (odin_vulkan_pipeline*)pipeline;
 
    /* Bind the pipeline */
    vkCmdBindPipeline(vulkan_command_buffer->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan_pipeline->pipeline);

    /* Bind the uniform buffers */
    vkCmdBindDescriptorSets(vulkan_command_buffer->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan_pipeline->layout, 0, vulkan_pipeline->descriptor_set_count, vulkan_pipeline->descriptor_sets, 0, NULL);
}

/* Binds a vertex buffer */
void odin_vulkan_draw_set_buffer_vertex            
(odin_data* data, odin_buffer_command* command_buffer, odin_buffer_vertex* vertex_buffer)
{
    /* Get the odin_vulkan_buffer_command */
    odin_vulkan_buffer_command* vulkan_command_buffer = (odin_vulkan_buffer_command*)command_buffer;

    /* Get the odin_vulkan_buffer_vertex */
    odin_vulkan_buffer_vertex* vulkan_vertex_buffer = (odin_vulkan_buffer_vertex*)vertex_buffer;

    /* Bind the vertex buffer */
    VkDeviceSize offsets = {0};
    vkCmdBindVertexBuffers(vulkan_command_buffer->buffer, 0, 1, &vulkan_vertex_buffer->buffer, &offsets);
}

/* Binds an index buffer */
void odin_vulkan_draw_set_buffer_index             
(odin_data* data, odin_buffer_command* command_buffer, odin_buffer_index* index_buffer)
{
    /* Get the odin_vulkan_buffer_command */
    odin_vulkan_buffer_command* vulkan_command_buffer = (odin_vulkan_buffer_command*)command_buffer;

    /* Get the odin_vulkan_buffer_index */
    odin_vulkan_buffer_index* vulkan_index_buffer = (odin_vulkan_buffer_index*)index_buffer;

    /* Bind the index buffer */
    VkDeviceSize offset = {0};
    vkCmdBindIndexBuffer(vulkan_command_buffer->buffer, vulkan_index_buffer->buffer, offset, VK_INDEX_TYPE_UINT32);
}

/* Submits the frame to drawing on the gpu */
extern void odin_vulkan_draw_indexed                      
(odin_data* data, odin_buffer_command* command_buffer, uint32_t indexCount)
{
    /* Get the odin_vulkan_buffer_command */
    odin_vulkan_buffer_command* vulkan_command_buffer = (odin_vulkan_buffer_command*)command_buffer;

    /* Execute the vulkan draw command */
    vkCmdDrawIndexed(vulkan_command_buffer->buffer, indexCount, 1, 0, 0, 0);

}

/* When rendering is to stop */
extern void odin_vulkan_draw_done                         
(odin_data* data)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Wait for device idle */
    vkDeviceWaitIdle(vulkan_data->device);

    /* Destroy the fences */
    for(uint32_t i = 0; i < vulkan_data->platform_data->swapchain_images_count; i++)
    {
        vkDestroyFence(vulkan_data->device, vulkan_data->in_flight_fences[i], NULL);
    }

    /* Destroy the semaphores */
    for(uint32_t i = 0; i < vulkan_data->platform_data->swapchain_images_count; i++)
    {
        vkDestroySemaphore(vulkan_data->device, vulkan_data->image_available_semaphores[i], NULL);
        vkDestroySemaphore(vulkan_data->device, vulkan_data->render_finished_semaphores[i], NULL);
    }

    /* Destroy the renderpass */
    vkDestroyRenderPass(vulkan_data->device, vulkan_data->render_pass, NULL);

}

static uint32_t current_frame = 0;


/* Tells the api when to draw a frame */
extern void odin_vulkan_draw_frame
(odin_data* data)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Wait for the frame of this index to finish */
    vkWaitForFences(vulkan_data->device, 1, &vulkan_data->in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);

    /* Get the render image from the swapchain */
    uint32_t image_index = 0;
    VkResult result = vkAcquireNextImageKHR(vulkan_data->device, vulkan_data->platform_data->swapchain, UINT64_MAX, vulkan_data->image_available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);

    /* Check if the swapchain needs to be resized */
    if(result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        odin_vulkan_platform_recreate_swapchain(vulkan_data);
        return;
    }

    /* Check if a previous frame is being used */
    if(vulkan_data->images_in_flight[image_index] != VK_NULL_HANDLE)
    {
        vkWaitForFences(vulkan_data->device, 1, &vulkan_data->images_in_flight[image_index], VK_TRUE, UINT64_MAX);
    }

    /* Mark the image as now being used */
    vulkan_data->images_in_flight[image_index] = vulkan_data->in_flight_fences[current_frame];

    /* Begin the command buffer */
    VkCommandBufferBeginInfo begin_info = { 0 };
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.pNext = NULL;
    begin_info.flags = 0;
    begin_info.pInheritanceInfo = NULL;

    VK_CHECK(vkBeginCommandBuffer(vulkan_data->command_buffers[image_index].buffer, &begin_info), "Could not begin recording a command buffer!");

    /* Set the viewport */
    VkViewport viewport = { 0 };
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)vulkan_data->platform_data->extent.width;
    viewport.height = (float)vulkan_data->platform_data->extent.height;
    viewport.maxDepth = 1.0f;
    viewport.minDepth = 0.0f;

    vkCmdSetViewport(vulkan_data->command_buffers[image_index].buffer, 0, 1, &viewport);


    /* Set the scissor */
    VkRect2D scissor = { 0 };
    scissor.extent = vulkan_data->platform_data->extent;
    scissor.offset = (VkOffset2D){0, 0};

    vkCmdSetScissor(vulkan_data->command_buffers[image_index].buffer, 0, 1, &scissor);

    /* Do the draw function */
    vulkan_data->odin_current_draw_func(data, (odin_buffer_command*)&vulkan_data->command_buffers[image_index]);

    VK_CHECK(vkEndCommandBuffer(vulkan_data->command_buffers[image_index].buffer), "Could not end a recording command buffer!");

    VkSemaphore wait_semaphores[] = { vulkan_data->image_available_semaphores[current_frame] };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSemaphore signal_semaphores[] = { vulkan_data->render_finished_semaphores[current_frame] };

    /* Build the submit info */
    VkSubmitInfo submit_info = { 0 };
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &vulkan_data->command_buffers[image_index].buffer;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    vkResetFences(vulkan_data->device, 1, &vulkan_data->in_flight_fences[current_frame]);

    /* Submit this queue */
    VK_CHECK(vkQueueSubmit(vulkan_data->graphics_queue, 1, &submit_info, vulkan_data->in_flight_fences[current_frame]), "Could not submit a draw command buffer!");

    VkSwapchainKHR swapchains[] = { vulkan_data->platform_data->swapchain };

    /* Build the present info */
    VkPresentInfoKHR present_info = { 0 };
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swapchains;
    present_info.pImageIndices = &image_index;
    present_info.pResults = NULL;

    /* Present the image */
    result = vkQueuePresentKHR(vulkan_data->present_queue, &present_info);

    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||  vulkan_data->swapchain_out_of_date)
    {
        vulkan_data->swapchain_out_of_date = false;
        odin_vulkan_platform_recreate_swapchain(vulkan_data);
    }

    /* Update the frame count */
    current_frame = (current_frame + 1) % vulkan_data->image_count;

    /* Wait until the queue is finished */
    vkQueueWaitIdle(vulkan_data->present_queue);


}