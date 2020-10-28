#include "o_vulkan_image.h"

#include "o_vulkan.h"
#include "o_vulkan_buffer.h"
#include "o_vulkan_buffer_command.h"
#include "vma.h"

#include "Aero/a_memory.h"


/* Creates an image with info */
extern void odin_vulkan_specific_image_create
(odin_vulkan_data* vulkan_data, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage_flags, VkMemoryPropertyFlags memory_properties, VkImage* image, VmaAllocation* allocation)
{
    /* Get the image size */
    const VkDeviceSize image_size = width * height * 4;

    /* Build the allocation create info */
    VmaAllocationCreateInfo allocation_create_info = { 0 };
    allocation_create_info.flags = 0;
    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    allocation_create_info.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    allocation_create_info.preferredFlags = 0;
    allocation_create_info.memoryTypeBits = 0;
    allocation_create_info.pool = VK_NULL_HANDLE;
    allocation_create_info.pUserData = NULL;

    /* Built the image_create_info */
    VkImageCreateInfo image_create_info = { 0 };
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.pNext = NULL;
    image_create_info.flags = 0;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.format = format;
    image_create_info.extent = (VkExtent3D){width, height, 1};
    image_create_info.mipLevels = 1;
    image_create_info.arrayLayers = 1;
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.tiling = tiling;
    image_create_info.usage = usage_flags;
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_create_info.queueFamilyIndexCount = 0;
    image_create_info.pQueueFamilyIndices = NULL;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    /* Create and allocate image */
    VK_CHECK(vmaCreateImage(vulkan_data->memory_allocator, &image_create_info, &allocation_create_info, image, allocation, NULL), "Could not create an image!");
}

/* Creates an image view */
void odin_vulkan_specific_image_view_create
(odin_vulkan_data* vulkan_data, VkImage image, VkFormat format, VkImageAspectFlags aspect_flags, VkImageView* image_view)
{ 
    /* Build the image view create info */
    VkImageViewCreateInfo create_info = { 0 };
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.pNext = NULL;
    create_info.flags = 0;
    create_info.image = image;
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = aspect_flags;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    /* Create the view */
    VK_CHECK(vkCreateImageView(vulkan_data->device, &create_info, NULL, image_view), "Could not create an image view!");
}

/* Transitions an image to become a different layout */
extern void odin_vulkan_specific_transition_image_layout
(odin_vulkan_data* vulkan_data, VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout)
{
    /* Create the single time command buffer */
    VkCommandBuffer command_buffer = odin_vulkan_specific_begin_single_time_commands(vulkan_data);

    
    VkPipelineStageFlags source_stage;
    VkPipelineStageFlags destination_stage;
    VkAccessFlags srcAccessMask = 0;
    VkAccessFlags dstAccessMask = 0;

    if(old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        srcAccessMask = 0;
        dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if(old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        assert(false);
    }
    


    /* Build the image barrior info */
    VkImageMemoryBarrier barrier = { 0 };
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.pNext = NULL;
    barrier.srcAccessMask = srcAccessMask;
    barrier.dstAccessMask = dstAccessMask;
    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    vkCmdPipelineBarrier(
        command_buffer, 
        source_stage, destination_stage,
        0,
        0, NULL,
        0, NULL,
        1, &barrier);

    /* End the single time commands */
    odin_vulkan_specific_end_single_time_commands(vulkan_data, command_buffer);

}

/* Copys a buffers data to an image */
extern void odin_vulkan_specific_buffer_copy_to_image
(odin_vulkan_data* vulkan_data, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    /* Begin the single time commands */
    VkCommandBuffer command_buffer = odin_vulkan_specific_begin_single_time_commands(vulkan_data);

    /* Build the image buffer copy region */
    VkBufferImageCopy region = { 0 };
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = (VkOffset3D){0, 0, 0};
    region.imageExtent = (VkExtent3D){width, height, 1};

    vkCmdCopyBufferToImage(
        command_buffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region);

    /* End the single time commands */
    odin_vulkan_specific_end_single_time_commands(vulkan_data,command_buffer);
}

/* Creates a image sampler */
void odin_vulkan_specific_sampler_create
(odin_vulkan_data* vulkan_data, VkSampler* sampler)
{
    /* Build the sampler create info */
    VkSamplerCreateInfo create_info = { 0 };
    create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    create_info.pNext = NULL;
    create_info.flags = 0;
    create_info.magFilter = VK_FILTER_LINEAR;
    create_info.minFilter = VK_FILTER_LINEAR;
    create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    create_info.mipLodBias = 0.0f;
    create_info.anisotropyEnable = VK_TRUE;
    create_info.maxAnisotropy = 16.0f;
    create_info.compareEnable = VK_FALSE;
    create_info.compareOp = VK_COMPARE_OP_ALWAYS;
    create_info.minLod = 0.0f;
    create_info.maxLod = 0.0f;
    create_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    create_info.unnormalizedCoordinates = VK_FALSE;

    /* Create the sampler */
    VK_CHECK(vkCreateSampler(vulkan_data->device, &create_info, NULL, sampler), "Could not create a sampler!")
}