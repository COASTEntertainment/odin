#include "o_vulkan_image_2d.h"

#include "o_vulkan.h"
#include "o_vulkan_buffer.h"
#include "o_vulkan_image.h"

#include "../helpers/stb_image.h"


/* Creates an image from a file */
odin_image_2d* odin_vulkan_image_2d_create_and_load
(odin_data* data, const char* path)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Load the image through stb */
    int width = 0, height = 0, channels = 0; 
    stbi_uc* pixels = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

    /* Error checking */
    if(!pixels)
    {
        LOG_ERROR("%s, %s", "Could not load an image", path);
    }

    /* Get the image size */
    VkDeviceSize image_size = width * height * 4;

    /* Create the staging buffer */
    VkBuffer image_staging_buffer = VK_NULL_HANDLE;
    VmaAllocation image_staging_allocation;
    VmaAllocationInfo image_staging_allocation_info = { 0 }; 

    odin_vulkan_specific_buffer_create(
        vulkan_data,
        image_size, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        VMA_MEMORY_USAGE_CPU_ONLY, 
        0, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        &image_staging_buffer, 
        &image_staging_allocation, 
        &image_staging_allocation_info);

    /* Copy the image data to the staging buffer */
    void* memory_data;
    vmaMapMemory(vulkan_data->memory_allocator, image_staging_allocation, &memory_data);
        memcpy(memory_data, pixels, image_size);
    vmaUnmapMemory(vulkan_data->memory_allocator, image_staging_allocation);

    /* Free the image memory as it is inside of the staging buffer */
    stbi_image_free(pixels);

    /* Create the image */
    VkImage image = VK_NULL_HANDLE;
    VmaAllocation image_allocation;

    odin_vulkan_specific_image_create(
        vulkan_data,
        width, 
        height, 
        VK_FORMAT_R8G8B8A8_SRGB, 
        VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &image,
        &image_allocation);

    /* Transition to the image copy layout */
    odin_vulkan_specific_transition_image_layout(
        vulkan_data,
        image,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        /* Copy the staging buffer to the image */
        odin_vulkan_specific_buffer_copy_to_image(
            vulkan_data,
            image_staging_buffer, 
            image,
            width,
            height);
    
    /* Transision back to the shader optimal layout */
    odin_vulkan_specific_transition_image_layout(
        vulkan_data,
        image,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    /* Destry the staging buffer */
    vmaDestroyBuffer(vulkan_data->memory_allocator, image_staging_buffer, image_staging_allocation);

    /* Create the image view */
    VkImageView image_view = VK_NULL_HANDLE;
    odin_vulkan_specific_image_view_create(vulkan_data, image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, &image_view);

    /* Create the image sampler */
    VkSampler image_sampler;
    odin_vulkan_specific_sampler_create(vulkan_data, &image_sampler);

    /* Make the odin_vulkan_image_2d */
    odin_vulkan_image_2d* vulkan_image_2d = (odin_vulkan_image_2d*)aero_malloc(sizeof(odin_vulkan_image_2d));
    vulkan_image_2d->image = image;
    vulkan_image_2d->allocation = image_allocation;
    vulkan_image_2d->image_view = image_view;
    vulkan_image_2d->image_sampler = image_sampler;

    return (odin_image_2d*)vulkan_image_2d;
}

/* Creates an image with data */ 
odin_image_2d* odin_vulkan_image_2d_create
(odin_data* data, int width, int height, odin_image_2d_tiling tiling_mode, void* image_data)
{
    

    //odin_vulkan_specific_image_create((uint32_t)width, (uint32_t)height, VK_FORMAT_R8G8B8A8_UNORM, VK_TIL  );
    return NULL;
}

/* Destroys an image and its memory */
void odin_vulkan_image_2d_destroy
(odin_data* data, odin_image_2d* image)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;
    
    /* Get the vulkan image */
    odin_vulkan_image_2d* vulkan_image = (odin_vulkan_image_2d*)image;

    vkDestroyImageView(vulkan_data->device, vulkan_image->image_view, NULL);
    vkDestroyImage(vulkan_data->device, vulkan_image->image, NULL);
}
