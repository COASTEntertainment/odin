#include "odin/apis/vulkan/o_vulkan_framebuffer.h"

#include "odin/apis/vulkan/o_vulkan_render_device.h"
#include "odin/apis/vulkan/o_vulkan_render_pass.h"
#include "odin/apis/vulkan/o_vulkan_texture_2d.h"


void odin_vulkan_framebuffer_create(odin_render_device render_device, odin_framebuffer* framebuffer, int width, int height, odin_render_pass render_pass, int attachments_count, odin_texture_2d* attachments)
{

    /* Get the vulkan objects. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    odin_vulkan_render_pass vulkan_render_pass = (odin_vulkan_render_pass)render_pass;

    /* Allocate the framebuffer object. */
    odin_vulkan_framebuffer vulkan_framebuffer = malloc(sizeof(odin_vulkan_framebuffer_t));
    *framebuffer = (odin_framebuffer)vulkan_framebuffer;




    /* Get all the attachments in a vulkan pointer. */
    VkImageView* vulkan_attachments = malloc(sizeof(VkImageView) * attachments_count);

    for(int i = 0; i < attachments_count; i++)
    {

        /* Get the vulkan texture. */
        odin_vulkan_texture_2d vulkan_texture = (odin_vulkan_texture_2d)attachments[i];
        
        vulkan_attachments[i] = vulkan_texture->image_view;

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


}

void odin_vulkan_framebuffer_destroy(odin_render_device render_device, odin_framebuffer framebuffer)
{

}

