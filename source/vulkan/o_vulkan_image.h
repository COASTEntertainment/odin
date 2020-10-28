#ifndef __VULKAN_IMAGE_H__
#define __VULKAN_IMAGE_H__


#include <vulkan/vulkan.h>

#include "o_vulkan_data.h"
#include "vma.h"


/* Creates an image with info */
extern void odin_vulkan_specific_image_create
(odin_vulkan_data* vulkan_data, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage_flags, VkMemoryPropertyFlags memory_properties, VkImage* image, VmaAllocation* allocation);

/* Creates an image view */
extern void odin_vulkan_specific_image_view_create
(odin_vulkan_data* vulkan_data, VkImage image, VkFormat format, VkImageAspectFlags aspect_flags, VkImageView* image_view);

/* Transitions an image to become a different layout */
extern void odin_vulkan_specific_transition_image_layout
(odin_vulkan_data* vulkan_data, VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout);

/* Copys a buffers data to an image */
extern void odin_vulkan_specific_buffer_copy_to_image
(odin_vulkan_data* vulkan_data, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

/* Creates a image sampler */
extern void odin_vulkan_specific_sampler_create
(odin_vulkan_data* vulkan_data, VkSampler* sampler);


#endif /* __VULKAN_IMAGE_H__*/