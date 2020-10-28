#ifndef __ODIN_VULKAN_BUFFER_H__
#define __ODIN_VULKAN_BUFFER_H__


#include <vulkan/vulkan.h>

#include "o_vulkan_data.h"
#include "vma.h"


/* Create a vulkan buffer */
void odin_vulkan_specific_buffer_create
(odin_vulkan_data* vulkan_data, VkDeviceSize buffer_size, VkBufferUsageFlags buffer_usage, VmaMemoryUsage memory_usage, VmaAllocationCreateFlags allocation_flags, VkMemoryPropertyFlags properties, VkBuffer* buffer, VmaAllocation* buffer_allocation, VmaAllocationInfo* allocation_info);

/* Copys a vulkan buffer to another */
void odin_vulkan_specific_buffer_copy
(odin_vulkan_data* vulkan_data, VkBuffer source_buffer, VkBuffer destination_buffer, VkDeviceSize size);




#endif /* __ODIN_VULKAN_BUFFER_H__ */