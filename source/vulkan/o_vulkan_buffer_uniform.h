#ifndef __ODIN_VULKAN_BUFFER_UNIFORM_H__
#define __ODIN_VULKAN_BUFFER_UNIFORM_H__


#include "o_vulkan_data.h"
#include "../o_buffer_uniform.h"

#include <vulkan/vulkan.h>
#include "vma.h"


typedef struct odin_vulkan_buffer_uniform
{
    aero_size_t buffer_size;
    VkBuffer buffer;
    VmaAllocation buffer_allocation;
    VkDescriptorSetLayoutBinding descriptor_binding;
    VkDescriptorSetLayout descriptor_layout;
    VkDescriptorSet descriptor_set;
} odin_vulkan_buffer_uniform;


/* Creates a uniform buffer object */
extern odin_buffer_uniform* odin_vulkan_uniform_create
(odin_data* data, odin_buffer_uniform_stage_type stage, aero_size_t size);

/* Destroys a uniform buffer */
extern void odin_vulkan_uniform_destroy
(odin_data* data, odin_buffer_uniform* uniform);

/* Changes the data in the uniform */
extern void odin_vulkan_uniform_update
(odin_data* data, odin_buffer_uniform* uniform, void* uniform_data);


#endif /* __ODIN_VULKAN_BUFFER_UNIFORM_H__ */