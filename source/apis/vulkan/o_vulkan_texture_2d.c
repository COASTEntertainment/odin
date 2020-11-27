#include "odin/apis/vulkan/o_vulkan_texture_2d.h"

#include <vulkan/vulkan.h>

#include "aero/a_memory.h"

#include "odin/o_render_device.h"
#include "odin/o_texture_2d.h"

#include "odin/apis/vulkan/vk_mem_alloc.h"
#include "odin/apis/vulkan/o_vulkan_render_device.h"


static VkFormat vulkan_formats[] = 
{

    VK_FORMAT_R8_UINT,
    VK_FORMAT_R16_UINT,
    VK_FORMAT_R32_UINT,

    VK_FORMAT_R8_SINT,
    VK_FORMAT_R16_SINT,
    VK_FORMAT_R32_SINT,

    VK_FORMAT_R8G8_UINT,
    VK_FORMAT_R16G16_UINT,
    VK_FORMAT_R32G32_UINT,

    VK_FORMAT_R16G16_SFLOAT,
    VK_FORMAT_R32G32_SFLOAT,
    
    VK_FORMAT_R16G16B16_SFLOAT,
    VK_FORMAT_R32G32B32_SFLOAT,

    VK_FORMAT_R16G16B16A16_SFLOAT,
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

void odin_vulkan_texture_2d_create(odin_render_device render_device, odin_texture_2d* texture, odin_texture_2d_image_format format, int width, int height, int mip_levels, odin_texture_2d_samples samples, int32_t size, void *data)
{

    /* Get the odin vulkan render device. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    /* Allocate the render device. */
    odin_vulkan_texture_2d vulkan_texture = malloc(sizeof(odin_vulkan_render_device_t));
    *texture = (odin_texture_2d)vulkan_texture;


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

    VkBuffer image_buffer = VK_NULL_HANDLE;
    VmaAllocation image_buffer_allocation = VK_NULL_HANDLE;

    VmaAllocationInfo allocation_info = { 0 };


    vmaCreateBuffer(vulkan_render_device->memory_allocator, &buffer_create_info, &allocation_create_info, &image_buffer, &image_buffer_allocation, &allocation_info);


    /* Copy the image data to the buffer. */
    void* mapped_ptr;
    vmaMapMemory(vulkan_render_device->memory_allocator, image_buffer_allocation, &mapped_ptr);
        aero_memcpy(mapped_ptr, size, data, size);
    vmaUnmapMemory(vulkan_render_device->memory_allocator, image_buffer_allocation);


    /* Create the vulkan image. */
    VkImageCreateInfo image_create_info = { 0 };
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.pNext = NULL;
    image_create_info.flags = 0;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.format = vulkan_formats[format];
    image_create_info.extent = (VkExtent3D){width, height, 1};
    image_create_info.mipLevels = mip_levels;
    image_create_info.arrayLayers = 1;
    image_create_info.samples = vulkan_samples[samples];
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_create_info.queueFamilyIndexCount = 0;
    image_create_info.pQueueFamilyIndices = NULL;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    vkCreateImage(vulkan_render_device->device, &image_create_info, NULL, &vulkan_texture->image);


    /* Allocate image memory. */
    VmaAllocationCreateInfo image_allocation_create_info = { 0 };
    image_allocation_create_info.flags = 0;
    image_allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    image_allocation_create_info.requiredFlags = 0;
    image_allocation_create_info.preferredFlags = 0;
    image_allocation_create_info.memoryTypeBits = 0;
    image_allocation_create_info.pool = VK_NULL_HANDLE;
    image_allocation_create_info.pUserData = NULL;

    VmaAllocation image_allocation;

    VmaAllocationInfo image_allocation_info = { 0 };

    vmaAllocateMemoryForImage(vulkan_render_device->memory_allocator, vulkan_texture->image, &image_allocation_create_info, &image_allocation, &image_allocation_info);


    /* Copy the image buffer to the image. */
    VkCommandBufferAllocateInfo command_buffer_allocate_info = { 0 };
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.pNext = NULL;
    command_buffer_allocate_info.commandPool = vulkan_render_device->command_pool;
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkCommandBuffer temporary_command_buffer = VK_NULL_HANDLE;

    vkAllocateCommandBuffers(vulkan_render_device->device, &command_buffer_allocate_info, &temporary_command_buffer);

    VkCommandBufferBeginInfo command_buffer_begin_info = { 0 };
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext = NULL;
    command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    command_buffer_begin_info.pInheritanceInfo = NULL;

    vkBeginCommandBuffer(temporary_command_buffer, &command_buffer_begin_info);

        VkBufferImageCopy region = { 0 };
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;

        vkCmdCopyBufferToImage(temporary_command_buffer, image_buffer, vulkan_texture->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
        
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



    /* Create the image view. */
    VkImageViewCreateInfo image_view_create_info = { 0 };
    image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    image_view_create_info.pNext = NULL;
    image_view_create_info.flags = 0;
    image_view_create_info.image = vulkan_texture->image;
    image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    image_view_create_info.format = vulkan_formats[format];
    image_view_create_info.components.r = 1.0f;
    image_view_create_info.components.g = 1.0f;
    image_view_create_info.components.b = 1.0f;
    image_view_create_info.components.a = 1.0f;
    image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT | VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    image_view_create_info.subresourceRange.baseMipLevel = 0;
    image_view_create_info.subresourceRange.levelCount = mip_levels;
    image_view_create_info.subresourceRange.baseArrayLayer = 1;
    image_view_create_info.subresourceRange.layerCount = 1;

    vkCreateImageView(vulkan_render_device->device, &image_view_create_info, NULL, &vulkan_texture->image_view);

    /* Generate the mip maps if requested. */
    if(mip_levels < 0)
    {

        

    }

    vkFreeCommandBuffers(vulkan_render_device->device, vulkan_render_device->command_pool, 1, &temporary_command_buffer);


}