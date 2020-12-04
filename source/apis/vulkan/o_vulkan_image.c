#include "odin/apis/vulkan/o_vulkan_image.h"

#include <vulkan/vulkan.h>

#include "aero/a_memory.h"

#include "odin/o_render_device.h"
#include "odin/o_image.h"

#include "odin/o_log.h"
#include "odin/apis/vulkan/o_vulkan_render_device.h"


uint32_t get_format_size(odin_image_format format)
{
    switch (format)
    {
    case odin_image_format_r_8_u_int:
        return sizeof(uint8_t);
        break;
    
    case odin_image_format_r_32_u_int:
        return sizeof(uint32_t);
        break;

    
    case odin_image_format_r_8_int:
        return sizeof(int8_t);
        break;


    case odin_image_format_r_32_int:
        return sizeof(int32_t);
        break;

    
    case odin_image_format_r_32_float:
        return sizeof(float);
        break;
    

    case odin_image_format_rg_8_u_int:
        return sizeof(uint8_t) * 2;
        break;
    
    case odin_image_format_rg_32_u_int:
        return sizeof(uint32_t) * 2;
        break;


    case odin_image_format_rgb_8_srgb:
        return sizeof(int8_t) * 3;
        break;

    case odin_image_format_rgb_32_float:
        return sizeof(float) * 3;
        break;

    case odin_image_format_rgba_8_srgb:
        return sizeof(int8_t) * 4;
        break;

    case odin_image_format_rgba_32_float:
        return sizeof(float) * 4;
        break;

    default:
        return 0;
        break;
    }
}


static VkFormat vulkan_formats[] = 
{

    VK_FORMAT_R8_UINT,
    VK_FORMAT_R32_UINT,

    VK_FORMAT_R8_SINT,
    VK_FORMAT_R32_SINT,

    VK_FORMAT_R32_SFLOAT,

    VK_FORMAT_R8G8_UINT,
    VK_FORMAT_R32G32_UINT,

    VK_FORMAT_R32G32_SFLOAT,

    VK_FORMAT_R8G8B8_SRGB,

    VK_FORMAT_R32G32B32_SFLOAT,

    VK_FORMAT_R8G8B8A8_SRGB,

    VK_FORMAT_R32G32B32A32_SFLOAT

};

static VkSampleCountFlags vulkan_samples[] =
{
    VK_SAMPLE_COUNT_1_BIT,
    VK_SAMPLE_COUNT_2_BIT,
    VK_SAMPLE_COUNT_4_BIT,
    VK_SAMPLE_COUNT_8_BIT,
    VK_SAMPLE_COUNT_16_BIT,
    VK_SAMPLE_COUNT_32_BIT,
    VK_SAMPLE_COUNT_64_BIT
};

void odin_vulkan_image_create(odin_render_device render_device, odin_image* image, odin_image_format format, int width, int height, int depth, int mip_levels, odin_image_samples samples)
{

    /* Get the odin vulkan render device. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    /* Allocate the render device. */
    odin_vulkan_image vulkan_image = malloc(sizeof(odin_vulkan_render_device_t));
    *image = (odin_image)vulkan_image;

    /* Check the image create info limits. */
    VkPhysicalDeviceProperties properties = { 0 };
    vkGetPhysicalDeviceProperties(vulkan_render_device->physical_device, &properties);

    VkImageFormatProperties image_format_properties = { 0 };
    VkResult res = vkGetPhysicalDeviceImageFormatProperties(vulkan_render_device->physical_device, vulkan_formats[format], VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, 0, &image_format_properties);

    if(width <= 0 || height <= 0 || depth <= 0)
    {
        ODIN_ERROR("o_vulkan_image.c", "Image must have have a width, height and depth of at least 1.");
    }

    if(width > image_format_properties.maxExtent.width || height > image_format_properties.maxExtent.height)
    {
        ODIN_ERROR("o_vulkan_image.c", "Image exceeds max physical device limits.");
    }

    if(mip_levels > image_format_properties.maxMipLevels)
    {
        ODIN_ERROR("o_vulkan_image.c", "Image exceeds max mip levels.");
    }

    if(samples > odin_image_samples_1x && mip_levels > 1)
    {
        ODIN_ERROR("o_vulkan_image.c", "Can not have mips with samples greater than 1x!");
    }

    vulkan_image->o_width       = width;
    vulkan_image->o_height      = height;
    vulkan_image->o_depth       = depth;
    vulkan_image->o_format      = format;
    vulkan_image->o_mip_levels  = mip_levels;

    vulkan_image->format    = vulkan_formats[format];
    vulkan_image->samples   = vulkan_samples[samples];

    
    VkImageType image_type = VK_IMAGE_TYPE_2D;

    if(depth > 1)
    {
        image_type = VK_IMAGE_TYPE_3D;
    }


    /* Create the vulkan image. */
    VkImageCreateInfo image_create_info = { 0 };
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.pNext = NULL;
    image_create_info.flags = 0;
    image_create_info.imageType = image_type;
    image_create_info.format = vulkan_formats[format];
    image_create_info.extent = (VkExtent3D){width, height, depth};
    image_create_info.mipLevels = mip_levels;
    image_create_info.arrayLayers = 1;
    image_create_info.samples = vulkan_samples[samples];
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_create_info.queueFamilyIndexCount = 0;
    image_create_info.pQueueFamilyIndices = NULL;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    vkCreateImage(vulkan_render_device->device, &image_create_info, NULL, &vulkan_image->image);


    /* Allocate image memory. */
    VmaAllocationCreateInfo image_allocation_create_info = { 0 };
    image_allocation_create_info.flags = 0;
    image_allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    image_allocation_create_info.requiredFlags = 0;
    image_allocation_create_info.preferredFlags = 0;
    image_allocation_create_info.memoryTypeBits = 0;
    image_allocation_create_info.pool = VK_NULL_HANDLE;
    image_allocation_create_info.pUserData = NULL;

    VmaAllocationInfo image_allocation_info = { 0 };

    vmaAllocateMemoryForImage(vulkan_render_device->memory_allocator, vulkan_image->image, &image_allocation_create_info, &vulkan_image->image_allocation, &image_allocation_info);

    /* Bind the image memory so that it can be written to. */
    vmaBindImageMemory(vulkan_render_device->memory_allocator, vulkan_image->image_allocation, vulkan_image->image);

    /* Create the image view. */
    VkImageViewCreateInfo image_view_create_info = { 0 };
    image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    image_view_create_info.pNext = NULL;
    image_view_create_info.flags = 0;
    image_view_create_info.image = vulkan_image->image;
    image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    image_view_create_info.format = vulkan_formats[format];
    image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    image_view_create_info.subresourceRange.baseMipLevel = 0;
    image_view_create_info.subresourceRange.levelCount = mip_levels;
    image_view_create_info.subresourceRange.baseArrayLayer = 0;
    image_view_create_info.subresourceRange.layerCount = 1;

    vkCreateImageView(vulkan_render_device->device, &image_view_create_info, NULL, &vulkan_image->image_view);

}


void odin_vulkan_image_upload_data(odin_render_device render_device, odin_image image, void* data)
{

    /* Get the odin vulkan render device. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    odin_vulkan_image vulkan_image = (odin_vulkan_image)image;

    int size = vulkan_image->o_width * vulkan_image->o_height * vulkan_image->o_depth * get_format_size(vulkan_image->o_format);




    /* Create an image buffer. */
    VkBufferCreateInfo buffer_create_info = { 0 };
    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.pNext = NULL;
    buffer_create_info.flags = 0;
    buffer_create_info.size = size;
    buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buffer_create_info.queueFamilyIndexCount = 0;
    buffer_create_info.pQueueFamilyIndices = NULL;

    VmaAllocationCreateInfo allocation_create_info = { 0 };
    allocation_create_info.flags = 0;
    allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;
    allocation_create_info.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    allocation_create_info.preferredFlags = 0;
    allocation_create_info.memoryTypeBits = 0;
    allocation_create_info.pool = VK_NULL_HANDLE;
    allocation_create_info.pUserData = NULL;

    VkBuffer staging_buffer = VK_NULL_HANDLE;
    VmaAllocation staging_buffer_allocation = VK_NULL_HANDLE;

    VmaAllocationInfo allocation_info = { 0 };


    VkResult res =  vmaCreateBuffer(vulkan_render_device->memory_allocator, &buffer_create_info, &allocation_create_info, &staging_buffer, &staging_buffer_allocation, &allocation_info);


    /* Copy the image data to the buffer. */
    void* mapped_ptr;
    vmaMapMemory(vulkan_render_device->memory_allocator, staging_buffer_allocation, &mapped_ptr);

        aero_memcpy(mapped_ptr, size, data, size);

    vmaUnmapMemory(vulkan_render_device->memory_allocator, staging_buffer_allocation);



    /* Copy the image buffer to the image. */
    VkCommandBufferAllocateInfo command_buffer_allocate_info = { 0 };
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.pNext = NULL;
    command_buffer_allocate_info.commandPool = vulkan_render_device->command_pool;
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount = 1;

    VkCommandBuffer temporary_command_buffer = VK_NULL_HANDLE;

    vkAllocateCommandBuffers(vulkan_render_device->device, &command_buffer_allocate_info, &temporary_command_buffer);

    VkCommandBufferBeginInfo command_buffer_begin_info = { 0 };
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext = NULL;
    command_buffer_begin_info.flags = 0;
    command_buffer_begin_info.pInheritanceInfo = NULL;


    vkBeginCommandBuffer(temporary_command_buffer, &command_buffer_begin_info);

        /* Transition the image layout to copy the image. */
        VkImageMemoryBarrier barrier = { 0 };
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.pNext = NULL;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = vulkan_image->image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = vulkan_image->o_mip_levels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(temporary_command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);


        VkBufferImageCopy region = { 0 };
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = (VkOffset3D){ 0, 0, 0 };
        region.imageExtent = (VkExtent3D){ vulkan_image->o_width, vulkan_image->o_height, vulkan_image->o_depth };

        vkCmdCopyBufferToImage(temporary_command_buffer, staging_buffer, vulkan_image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
        
    vkEndCommandBuffer(temporary_command_buffer);

    VkSubmitInfo submit_info = { 0 };
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = NULL;
    submit_info.waitSemaphoreCount = 0;
    submit_info.pWaitSemaphores = NULL;
    submit_info.pWaitDstStageMask = NULL;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &temporary_command_buffer;
    submit_info.signalSemaphoreCount = 0;
    submit_info.pSignalSemaphores = NULL;
    
    vkQueueSubmit(vulkan_render_device->graphics_queue, 1, &submit_info, VK_NULL_HANDLE);

    /* Wait for the queue to finish. */
    vkQueueWaitIdle(vulkan_render_device->graphics_queue);

    vkResetCommandBuffer(temporary_command_buffer, 0);

    /* Destroy the staging buffer. */
    vmaDestroyBuffer(vulkan_render_device->memory_allocator, staging_buffer, staging_buffer_allocation);

    
    
    /* Generate the mip maps if requested. */
    vkBeginCommandBuffer(temporary_command_buffer, &command_buffer_begin_info);

        int mip_width = vulkan_image->o_width;
        int mip_height = vulkan_image->o_height;
        int mip_depth = vulkan_image->o_depth; 

        for(int i = 1; i < vulkan_image->o_mip_levels; i++)
        {
            
            VkImageMemoryBarrier barrier = { 0 };
            barrier.sType                               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.pNext                               = NULL;
            barrier.srcAccessMask                       = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask                       = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.oldLayout                           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout                           = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcQueueFamilyIndex                 = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex                 = VK_QUEUE_FAMILY_IGNORED;
            barrier.image                               = vulkan_image->image;
            barrier.subresourceRange.aspectMask         = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel       = i - 1;
            barrier.subresourceRange.levelCount         = 1;
            barrier.subresourceRange.baseArrayLayer     = 0;
            barrier.subresourceRange.layerCount         = 1;

            vkCmdPipelineBarrier(temporary_command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);

            /* Blit the image to get the desired mip effect. */
            VkImageBlit blit = { 0 };
            blit.srcSubresource.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel        = i - 1;
            blit.srcSubresource.baseArrayLayer  = 0;
            blit.srcSubresource.layerCount      = 1;
            blit.srcOffsets[0]                  = (VkOffset3D){ 0, 0, 0 };
            blit.srcOffsets[1]                  = (VkOffset3D){ mip_width, mip_height, mip_depth };
            blit.dstSubresource.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel        = i;
            blit.dstSubresource.baseArrayLayer  = 0;
            blit.dstSubresource.layerCount      = 1;
            blit.dstOffsets[0]                  = (VkOffset3D){ 0, 0, 0 };
            /* If the width is greater than one, divide it by 2. That is what these ternary op's are doing. */
            blit.dstOffsets[1]                  = (VkOffset3D){ mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, mip_depth > 1 ? mip_depth / 2 : 1 }; 

            vkCmdBlitImage(temporary_command_buffer, vulkan_image->image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vulkan_image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

            /* Transition the mip to a shader optimal layout. */
            barrier.oldLayout       = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask   = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask   = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(temporary_command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);

            if(mip_width > 1) mip_width /= 2;
            if(mip_height > 1) mip_height /= 2;
            if(mip_depth > 1) mip_depth /= 2;

        }

    vkEndCommandBuffer(temporary_command_buffer);

    vkQueueSubmit(vulkan_render_device->graphics_queue, 1, &submit_info, VK_NULL_HANDLE);

    vkQueueWaitIdle(vulkan_render_device->graphics_queue);

    /* Free the temporary command buffer. */
    vkFreeCommandBuffers(vulkan_render_device->device, vulkan_render_device->command_pool, 1, &temporary_command_buffer);

}


void odin_vulkan_image_destroy(odin_render_device render_device, odin_image image)
{

    /* Get the odin vulkan render device. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    /* Allocate the render device. */
    odin_vulkan_image vulkan_image = (odin_vulkan_image)image;

    vkDestroyImageView(vulkan_render_device->device, vulkan_image->image_view, NULL);

    vmaDestroyImage(vulkan_render_device->memory_allocator, vulkan_image->image, vulkan_image->image_allocation);

    free(vulkan_image);
    vulkan_image = NULL;

}