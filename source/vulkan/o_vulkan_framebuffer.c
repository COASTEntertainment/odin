#include "o_vulkan_framebuffer.h"

#include "Aero/a_memory.h"

#include "o_vulkan.h"
#include "o_vulkan_platform.h"
#include "o_vulkan_image.h"


/* Creates a framebuffer */
odin_vulkan_buffer_frame* odin_vulkan_buffer_frame_create
(odin_data* data, odin_renderpass* renderpass, uint32_t width, uint32_t height)
{
    return NULL;
}

/* Creates a framebuffer from an image */
odin_vulkan_framebuffer* odin_vulkan_specific_buffer_frame_create_from_image
(odin_vulkan_data* vulkan_data, VkRenderPass render_pass, VkExtent2D extent, VkImageAspectFlags image_aspect_flags, VkImage image)
{

    /* Create the framebuffer image view */
    VkImageView image_view;
    odin_vulkan_specific_image_view_create(vulkan_data, image, vulkan_data->platform_data->format.format, image_aspect_flags, &image_view);

    /* Build the framebuffer create info */
    VkFramebufferCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.renderPass = render_pass;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &image_view;
    createInfo.width = extent.width;
    createInfo.height = extent.height;
    createInfo.layers = 1;
    
    /* Create the framebuffer */
    VkFramebuffer framebuffer;
    VK_CHECK(vkCreateFramebuffer(vulkan_data->device, &createInfo, NULL, &framebuffer), "Could not create a framebuffer!");

    /* Now create the odin framebuffer and return */
    odin_vulkan_framebuffer* new_framebuffer = (odin_vulkan_framebuffer*)aero_malloc(sizeof(odin_vulkan_framebuffer));
    new_framebuffer->framebuffer = framebuffer;
    new_framebuffer->image = image;
    new_framebuffer->image_view = image_view;

    return new_framebuffer;
}

/* Destroys a framebuffer */
void odin_vulkan_buffer_frame_destroy
(odin_vulkan_data* data, odin_vulkan_framebuffer* framebuffer)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Get the vulkan framebuffer */
    odin_vulkan_buffer_frame* vulkan_buffer_frame = (odin_vulkan_buffer_frame*)framebuffer;

    /* Destroy all created objects */
    vkDestroyFramebuffer(vulkan_data->device, framebuffer->framebuffer, NULL);
    vkDestroyImageView(vulkan_data->device, framebuffer->image_view, NULL);
    vkDestroyImage(vulkan_data->device, framebuffer->image, NULL);
}

/* Destroys a custom framebuffer */
void odin_vulkan_specific_buffer_frame_destroy_from_image
(odin_vulkan_data* vulkan_data, odin_vulkan_framebuffer* framebuffer)
{

    /* Destroy all created objects */
    vkDestroyFramebuffer(vulkan_data->device, framebuffer->framebuffer, NULL);
    vkDestroyImageView(vulkan_data->device, framebuffer->image_view, NULL);
}