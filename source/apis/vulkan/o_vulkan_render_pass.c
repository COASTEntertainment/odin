#include "odin/apis/vulkan/o_vulkan_render_pass.h"


void odin_vulkan_render_pass_create(odin_render_device render_device, odin_render_pass* render_pass)
{



    VkAttachmentDescription attachment_description = { 0 };
    attachment_description.flags = 0;

    


    VkRenderPassCreateInfo render_pass_create_info = { 0 };
    render_pass_create_info.sType                   = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.pNext                   = NULL;
    render_pass_create_info.flags                   = 0;
    render_pass_create_info.attachmentCount         = 1;
    render_pass_create_info.pAttachments;

}

