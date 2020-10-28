#ifndef __ODIN_VULKAN_BUFFER_UNIFORM_H__
#define __ODIN_VULKAN_BUFFER_UNIFORM_H__


#include "stdint.h"

#include "o_vulkan_data.h"
#include "../o_uniform_buffer.h"

#include <vulkan/vulkan.h>
#include "vma.h"


typedef struct odin_vulkan_uniform_buffer
{

    uint32_t buffer_size;
    VkBuffer buffer;
    VmaAllocation buffer_allocation;

    VkDescriptorSetLayoutBinding descriptor_binding;
    VkDescriptorSetLayout descriptor_layout;
    VkDescriptorSet descriptor_set;

} odin_vulkan_uniform_buffer;


/* Creates a uniform buffer object */
extern odin_uniform_buffer* odin_vulkan_uniform_buffer_create
(odin_data* data, odin_uniform_buffer_stage_type stage, uint32_t size);

/* Destroys a uniform buffer */
extern void odin_vulkan_uniform_buffer_destroy
(odin_data* data, odin_uniform_buffer* uniform);

/* Changes the data in the uniform */
extern void odin_vulkan_uniform_buffer_update
(odin_data* data, odin_uniform_buffer* uniform, void* uniform_data);


#endif /* __ODIN_VULKAN_BUFFER_UNIFORM_H__ */