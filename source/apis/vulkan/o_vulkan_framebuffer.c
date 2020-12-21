#include "odin/apis/vulkan/o_vulkan_framebuffer.h"

#include "aero/a_memory.h"

#include "odin/o_log.h"
#include "odin/apis/vulkan/o_vulkan_render_device.h"
#include "odin/apis/vulkan/o_vulkan_render_pass.h"
#include "odin/apis/vulkan/o_vulkan_image.h"


void odin_vulkan_framebuffer_create(odin_render_device render_device, odin_framebuffer* framebuffer, int width, int height, odin_render_pass render_pass, int attachments_count, odin_image* attachments)
{

    /* Get the vulkan objects. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;
    odin_vulkan_render_pass vulkan_render_pass = (odin_vulkan_render_pass)render_pass;

    /* Allocate the framebuffer object. */
    odin_vulkan_framebuffer vulkan_framebuffer = malloc(sizeof(odin_vulkan_framebuffer_t));
    *framebuffer = (odin_framebuffer)vulkan_framebuffer;




    /* Get all the attachments in a vulkan pointer. */
    VkImageView* vulkan_attachments = NEW(VkImageView, attachments_count);

    for(int i = 0; i < attachments_count; i++)
    {

        /* Copy the image views to the vulkan attachments. */
        aero_memcpy(vulkan_attachments[i], sizeof(VkImageView), ((odin_vulkan_image)attachments[i])->image_view, sizeof(VkImageView));
    }


    VkFramebufferCreateInfo framebuffer_create_info = { 0 };
    framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_create_info.pNext = NULL;
    framebuffer_create_info.flags = 0;
    framebuffer_create_info.renderPass = vulkan_render_pass->render_pass;
    framebuffer_create_info.attachmentCount = attachments_count;
    framebuffer_create_info.pAttachments = vulkan_attachments;
    framebuffer_create_info.width = width;
    framebuffer_create_info.height = height;
    framebuffer_create_info.layers = 1;

    if (vkCreateFramebuffer(vulkan_render_device->device, &framebuffer_create_info, NULL, &vulkan_framebuffer->framebuffer) != VK_SUCCESS)
    {
        ODIN_ERROR("o_vulkan_framebuffer.c", "Could not create a framebuffer!");
    }

    DELETE(vulkan_attachments);

}

void odin_vulkan_framebuffer_destroy(odin_render_device render_device, odin_framebuffer framebuffer)
{

    /* Get the vulkan objects. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;
    odin_vulkan_framebuffer vulkan_framebuffer = (odin_vulkan_framebuffer)framebuffer;

    
    vkDestroyFramebuffer(vulkan_render_device->device, vulkan_framebuffer->framebuffer, NULL);

    DELETE(framebuffer);

}

