#include "odin/apis/vulkan/o_vulkan_render_pass.h"

#include "aero/a_memory.h"

#include "odin/apis/vulkan/o_vulkan_render_device.h"
#include "odin/apis/vulkan/o_vulkan_image.h"


void odin_vulkan_render_pass_create(odin_render_device render_device, odin_render_pass* render_pass, int attachments_count, odin_image* attachments)
{

    /* Get the vulkan objects. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    odin_vulkan_render_pass vulkan_render_pass = malloc(sizeof(odin_vulkan_render_pass_t));
    *render_pass = (odin_render_pass)vulkan_render_pass;


    /* Allocate the attachments array. */
    VkAttachmentDescription* vulkan_attachments = malloc(sizeof(VkAttachmentDescription) * attachments_count);
    VkSubpassDescription* subpasses = malloc(sizeof(VkSubpassDescription) * attachments_count);


    /* Set the attachments. */
    for(int i = 0; i < attachments_count; i++)
    {

        /* Get the vulkan texture. */
        odin_vulkan_image vulkan_texture = (odin_vulkan_image)attachments[i];

        /* Generate the attachment. */
        VkAttachmentDescription attachment = { 0 };
        attachment.flags            = 0;
        attachment.format           = vulkan_texture->format;
        attachment.samples          = vulkan_texture->samples;
        attachment.loadOp           = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment.storeOp          = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp    = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment.stencilStoreOp   = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.initialLayout    = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment.finalLayout      = VK_IMAGE_LAYOUT_GENERAL;

        aero_memcpy(&vulkan_attachments[i], sizeof(VkAttachmentDescription), &attachment, sizeof(VkAttachmentDescription));

        VkAttachmentReference attachment_reference = { 0 };
        attachment_reference.attachment     = i;
        attachment_reference.layout         = VK_IMAGE_LAYOUT_GENERAL;

        VkSubpassDescription subpass = { 0 };
        subpass.flags                       = 0;
        subpass.pipelineBindPoint           = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.inputAttachmentCount        = 0;
        subpass.pInputAttachments           = NULL;
        subpass.colorAttachmentCount        = 1;
        subpass.pColorAttachments           = &attachment_reference;
        subpass.pResolveAttachments         = NULL;
        subpass.pDepthStencilAttachment     = NULL;
        subpass.preserveAttachmentCount     = 0;
        subpass.pPreserveAttachments        = NULL;

        aero_memcpy(&subpasses[i], sizeof(VkSubpassDescription), &subpass, sizeof(VkSubpassDescription));

    }


    VkRenderPassCreateInfo render_pass_create_info = { 0 };
    render_pass_create_info.sType                   = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.pNext                   = NULL;
    render_pass_create_info.flags                   = 0;
    render_pass_create_info.attachmentCount         = attachments_count;
    render_pass_create_info.pAttachments            = vulkan_attachments;
    render_pass_create_info.subpassCount            = attachments_count;
    render_pass_create_info.pSubpasses              = subpasses;
    render_pass_create_info.dependencyCount         = 0;
    render_pass_create_info.pDependencies           = NULL;

    VkResult res = vkCreateRenderPass(vulkan_render_device->device, &render_pass_create_info, NULL, &vulkan_render_pass->render_pass);

}


void odin_vulkan_render_pass_destroy(odin_render_device render_device, odin_render_pass render_pass)
{

    /* Get the vulkan objects. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    odin_vulkan_render_pass vulkan_render_pass = (odin_vulkan_render_pass)render_pass;


    vkDestroyRenderPass(vulkan_render_device->device, vulkan_render_pass->render_pass, NULL);

}
