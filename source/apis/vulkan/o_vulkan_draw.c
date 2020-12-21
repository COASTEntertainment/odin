#include "odin/apis/vulkan/o_vulkan_draw.h"

#include "aero/a_memory.h"

#include "odin/o_log.h"

#include "odin/apis/vulkan/o_vulkan_render_device.h"
#include "odin/apis/vulkan/o_vulkan_window.h"
#include "odin/apis/vulkan/o_vulkan_draw_data.h"
#include "odin/apis/vulkan/o_vulkan_render_pass.h"
#include "odin/apis/vulkan/o_vulkan_framebuffer.h"
#include "odin/apis/vulkan/o_vulkan_pipeline.h"
#include "odin/apis/vulkan/o_vulkan_vertex_buffer.h"
#include "odin/apis/vulkan/o_vulkan_index_buffer.h"

#include <vulkan/vulkan.h>


#define MAX_FRAMES_IN_FLIGHT 2


void odin_vulkan_draw_prepare_window(odin_render_device render_device, odin_window window, odin_render_pass* present_pass)
{

    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;
    odin_vulkan_window vulkan_window = (odin_vulkan_window)window;


    /* Create the semaphores and fences. */
    vulkan_window->image_available_semaphores =     NEW(VkSemaphore, MAX_FRAMES_IN_FLIGHT);
    vulkan_window->render_complete_semaphores =     NEW(VkSemaphore, MAX_FRAMES_IN_FLIGHT);
    vulkan_window->in_flight_fences =               NEW(VkFence, MAX_FRAMES_IN_FLIGHT);
    vulkan_window->images_in_flight =               NEW(VkFence, vulkan_window->image_count);

    VkSemaphoreCreateInfo semaphore_create_info = { 0 };
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = NULL;
    semaphore_create_info.flags = 0;
    
    VkFenceCreateInfo fence_create_info = { 0 };
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.pNext = NULL;
    fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkCreateSemaphore(vulkan_render_device->device, &semaphore_create_info, NULL, &vulkan_window->image_available_semaphores[i]);
        vkCreateSemaphore(vulkan_render_device->device, &semaphore_create_info, NULL, &vulkan_window->render_complete_semaphores[i]);
        vkCreateFence(vulkan_render_device->device, &fence_create_info, NULL, &vulkan_window->in_flight_fences[i]);
    }

    aero_memset(vulkan_window->images_in_flight, sizeof(VkFence) * vulkan_window->image_count, 0);


    /* Create the command buffers. */   
    vulkan_window->command_buffers = NEW(VkCommandBuffer, vulkan_window->image_count);

    VkCommandBufferAllocateInfo command_buffer_allocate_info = { 0 };
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.pNext = NULL;
    command_buffer_allocate_info.commandPool = vulkan_render_device->command_pool;
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount = vulkan_window->image_count;
    
    vkAllocateCommandBuffers(vulkan_render_device->device, &command_buffer_allocate_info, vulkan_window->command_buffers);

    /* Create the present render pass. */
    odin_vulkan_render_pass present_pass_ = NEW(odin_vulkan_render_pass, 1);
    vulkan_window->present_pass = present_pass_; 

    VkAttachmentDescription attachment_description = { 0 };
    attachment_description.flags = 0;
    attachment_description.format = vulkan_window->surface_format.format;
    attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference attachment_reference = { 0 };
    attachment_reference.attachment = 0;
    attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass_description = { 0 };
    subpass_description.flags = 0;
    subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass_description.inputAttachmentCount = 0;
    subpass_description.pInputAttachments = NULL;
    subpass_description.colorAttachmentCount = 1;
    subpass_description.pColorAttachments = &attachment_reference;
    subpass_description.pResolveAttachments = NULL;
    subpass_description.pDepthStencilAttachment = NULL;
    subpass_description.preserveAttachmentCount = 0;
    subpass_description.pPreserveAttachments = NULL;

    VkSubpassDependency dependency = { 0 };
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.dependencyFlags = 0;

    VkRenderPassCreateInfo render_pass_create_info = { 0 };
    render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.pNext = NULL;
    render_pass_create_info.flags = 0;
    render_pass_create_info.attachmentCount = 1;
    render_pass_create_info.pAttachments = &attachment_description;
    render_pass_create_info.subpassCount = 1;
    render_pass_create_info.pSubpasses = &subpass_description;
    render_pass_create_info.dependencyCount = 1;
    render_pass_create_info.pDependencies = &dependency;

    if (vkCreateRenderPass(vulkan_render_device->device, &render_pass_create_info, NULL, &vulkan_window->present_pass->render_pass) != VK_SUCCESS)
    {
        ODIN_ERROR("o_vulkan_draw.c", "Could not create a render pass!");
    }

    
    present_pass_->render_pass = vulkan_window->present_pass->render_pass;

    *present_pass = (odin_render_pass)present_pass_;


    /* Create the framebuffers and image views. */
    vulkan_window->image_views =            NEW(VkImageView, vulkan_window->image_count);
    vulkan_window->image_framebuffers =     NEW(VkFramebuffer, vulkan_window->image_count);

    for(int i = 0; i < vulkan_window->image_count; i++)
    {
        
        VkImageViewCreateInfo image_view_create_info = { 0 };
        image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.pNext = NULL;
        image_view_create_info.flags = 0;
        image_view_create_info.image = vulkan_window->images[i];
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format = vulkan_window->surface_format.format;
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel = 0;
        image_view_create_info.subresourceRange.levelCount = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount = 1;

        vkCreateImageView(vulkan_render_device->device, &image_view_create_info, NULL, &vulkan_window->image_views[i]);



        VkFramebufferCreateInfo framebuffer_create_info = { 0 };
        framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_create_info.pNext = NULL;
        framebuffer_create_info.flags = 0;
        framebuffer_create_info.renderPass = vulkan_window->present_pass->render_pass;
        framebuffer_create_info.attachmentCount = 1;
        framebuffer_create_info.pAttachments = &vulkan_window->image_views[i];
        framebuffer_create_info.width = vulkan_window->surface_extent.width;
        framebuffer_create_info.height = vulkan_window->surface_extent.height;
        framebuffer_create_info.layers = 1;

        vkCreateFramebuffer(vulkan_render_device->device, &framebuffer_create_info, NULL, &vulkan_window->image_framebuffers[i]);
    }
}

static int current_frame = 0;

void odin_vulkan_draw_frame_window(odin_render_device render_device, odin_window window, odin_draw_func draw_func, void* user_data)
{

    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;
    
    odin_vulkan_window vulkan_window = (odin_vulkan_window)window;

    vkWaitForFences(vulkan_render_device->device, 1, &vulkan_window->in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);


    uint32_t image_index;
    vkAcquireNextImageKHR(vulkan_render_device->device, vulkan_window->swapchain, UINT64_MAX, vulkan_window->image_available_semaphores[current_frame], NULL, &image_index);

    /* Check if a previous frame is using this image. */


    if(vulkan_window->images_in_flight[image_index] != VK_NULL_HANDLE)
    {
        VkFence test = vulkan_window->images_in_flight[image_index];

        vkWaitForFences(vulkan_render_device->device, 1, &vulkan_window->images_in_flight[current_frame], VK_TRUE, UINT64_MAX);
    }

    /* Mark this image as being used. */
    vulkan_window->images_in_flight[image_index] = vulkan_window->in_flight_fences[current_frame];

    VkCommandBufferBeginInfo command_buffer_begin_info = { 0 };
    command_buffer_begin_info.sType             = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext             = NULL;
    command_buffer_begin_info.flags             = 0;
    command_buffer_begin_info.pInheritanceInfo  = NULL;

    VkCommandBuffer current_command_buffer = vulkan_window->command_buffers[image_index];


    vkBeginCommandBuffer(current_command_buffer, &command_buffer_begin_info);

        /* Create the draw data before the draw */
        odin_vulkan_draw_data vulkan_draw_data = malloc(sizeof(odin_vulkan_draw_data_t));
        vulkan_draw_data->command_buffer = current_command_buffer;
        vulkan_draw_data->vulkan_window = vulkan_window;

        odin_vulkan_framebuffer present_framebuffer = malloc(sizeof(odin_vulkan_framebuffer));
        present_framebuffer->framebuffer = vulkan_window->image_framebuffers[image_index];



        draw_func(render_device, (odin_draw_data)vulkan_draw_data, (odin_render_pass)vulkan_window->present_pass, (odin_framebuffer)present_framebuffer, user_data);

        

    vkEndCommandBuffer(current_command_buffer);

    vkResetFences(vulkan_render_device->device, 1, &vulkan_window->in_flight_fences[current_frame]);

    /* Submit the command buffer. */
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submit_info = { 0 };
    submit_info.sType                   = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext                   = NULL;
    submit_info.waitSemaphoreCount      = 1;
    submit_info.pWaitSemaphores         = &vulkan_window->image_available_semaphores[current_frame];
    submit_info.pWaitDstStageMask       = wait_stages;
    submit_info.commandBufferCount      = 1;
    submit_info.pCommandBuffers         = &current_command_buffer;
    submit_info.signalSemaphoreCount    = 1;
    submit_info.pSignalSemaphores       = &vulkan_window->render_complete_semaphores[current_frame];

    vkQueueSubmit(vulkan_render_device->graphics_queue, 1, &submit_info, vulkan_window->in_flight_fences[current_frame]);

    free(present_framebuffer);

    VkPresentInfoKHR present_info = { 0 };
    present_info.sType                  = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.pNext                  = NULL;
    present_info.waitSemaphoreCount     = 1;
    present_info.pWaitSemaphores        = &vulkan_window->render_complete_semaphores[current_frame];
    present_info.swapchainCount         = 1;
    present_info.pSwapchains            = &vulkan_window->swapchain;
    present_info.pImageIndices          = &image_index;
    present_info.pResults               = NULL;

    vkQueuePresentKHR(vulkan_render_device->present_queue, &present_info);

    current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;

}

void odin_vulkan_draw_done_window(odin_render_device render_device, odin_window window)
{
    
    /* Get the vulkan objects. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;
    
    odin_vulkan_window vulkan_window = (odin_vulkan_window)window;


    for(int i = 0; i < vulkan_window->image_count; i++)
    {
        vkDestroyFramebuffer(vulkan_render_device->device, vulkan_window->image_framebuffers[i], NULL);
        vkDestroyImageView(vulkan_render_device->device, vulkan_window->image_views[i], NULL);
    }

    vkDestroyRenderPass(vulkan_render_device->device, vulkan_window->present_pass->render_pass, NULL);

    free(vulkan_window->present_pass);

    /* Destroy the command buffers. */
    vkFreeCommandBuffers(vulkan_render_device->device, vulkan_render_device->command_pool, vulkan_window->image_count, vulkan_window->command_buffers);

    /* Destroy the windows semaphores. */
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(vulkan_render_device->device, vulkan_window->image_available_semaphores[i], NULL);
        vkDestroySemaphore(vulkan_render_device->device, vulkan_window->render_complete_semaphores[i], NULL);
        vkDestroyFence(vulkan_render_device->device, vulkan_window->in_flight_fences[i], NULL);
    }

}


void odin_vulkan_draw_command_begin_render_pass(odin_render_device render_device, odin_draw_data draw_data, odin_render_pass render_pass, odin_framebuffer framebuffer)
{

    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;
    odin_vulkan_draw_data vulkan_draw_data = (odin_vulkan_draw_data)draw_data;
    odin_vulkan_render_pass vulkan_render_pass = (odin_vulkan_render_pass)render_pass;
    odin_vulkan_framebuffer vulkan_framebuffer = (odin_vulkan_framebuffer)framebuffer;

    VkClearValue clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };

    VkRenderPassBeginInfo render_pass_begin_info = { 0 };
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.pNext = NULL;
    render_pass_begin_info.renderPass = vulkan_render_pass->render_pass;
    render_pass_begin_info.framebuffer = vulkan_framebuffer->framebuffer;
    render_pass_begin_info.renderArea.offset = (VkOffset2D){ 0, 0 };
    render_pass_begin_info.renderArea.extent = vulkan_draw_data->vulkan_window->surface_extent;
    render_pass_begin_info.clearValueCount = 1;
    render_pass_begin_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(vulkan_draw_data->command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

}

void odin_vulkan_draw_command_end_render_pass(odin_render_device render_device, odin_draw_data draw_data, odin_render_pass render_pass)
{

    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;
    odin_vulkan_draw_data vulkan_draw_data = (odin_vulkan_draw_data)draw_data;
    odin_vulkan_render_pass vulkan_render_pass = (odin_vulkan_render_pass)render_pass;

    vkCmdEndRenderPass(vulkan_draw_data->command_buffer);

}

void odin_vulkan_draw_command_bind_pipeline(odin_render_device render_device, odin_draw_data draw_data, odin_pipeline pipeline)
{

    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;
    odin_vulkan_draw_data vulkan_draw_data = (odin_vulkan_draw_data)draw_data;
    odin_vulkan_pipeline vulkan_pipeline = (odin_vulkan_pipeline)pipeline;



    vkCmdBindPipeline(vulkan_draw_data->command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan_pipeline->pipeline);

}


void odin_vulkan_draw_command_bind_vertex_buffer(odin_render_device render_device, odin_draw_data draw_data, odin_vertex_buffer vertex_buffer)
{

    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;
    odin_vulkan_draw_data vulkan_draw_data = (odin_vulkan_draw_data)draw_data;
    odin_vulkan_vertex_buffer vulkan_vertex_buffer = (odin_vulkan_vertex_buffer)vertex_buffer;

    VkBuffer vertex_buffers[] =
    {
        vulkan_vertex_buffer->buffer
    };

    VkDeviceSize offsets[1] =
    {
        0
    };

    vkCmdBindVertexBuffers(vulkan_draw_data->command_buffer, 0, 1, vertex_buffers, offsets);

}

void odin_vulkan_draw_command_bind_index_buffer(odin_render_device render_device, odin_draw_data draw_data, odin_index_buffer index_buffer)
{

    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;
    odin_vulkan_draw_data vulkan_draw_data = (odin_vulkan_draw_data)draw_data;
    odin_vulkan_index_buffer vulkan_index_buffer = (odin_vulkan_index_buffer)index_buffer;

    vkCmdBindIndexBuffer(vulkan_draw_data->command_buffer, vulkan_index_buffer->buffer, 0, VK_INDEX_TYPE_UINT32);

}

void odin_vulkan_draw_command_indexed(odin_render_device render_device, odin_draw_data draw_data, int index_count)
{

    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;
    odin_vulkan_draw_data vulkan_draw_data = (odin_vulkan_draw_data)draw_data;

    vkCmdDrawIndexed(vulkan_draw_data->command_buffer, index_count, 1, 0, 0, 0);

}

