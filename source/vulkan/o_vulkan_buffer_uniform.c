#include "o_vulkan_buffer_uniform.h"

#include "Aero/a_memory.h"

#include "o_vulkan.h"
#include "o_vulkan_buffer.h"


/* Creates a uniform buffer object */
odin_buffer_uniform* odin_vulkan_uniform_create
(odin_data* data, odin_buffer_uniform_stage_type stage, aero_size_t buffer_size)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Create the uniform buffer */
    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation buffer_allocation;

    odin_vulkan_specific_buffer_create(
        vulkan_data,
        buffer_size,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        0,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        &buffer,
        &buffer_allocation,
        NULL);

    /* Build the descriptor bindings */
    VkDescriptorSetLayoutBinding descriptor_binding = { 0 };
    descriptor_binding.binding = 0;
    descriptor_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_binding.descriptorCount = 1;
    descriptor_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    descriptor_binding.pImmutableSamplers = NULL;


    /* Build the descriptor set layout create info */
    VkDescriptorSetLayoutCreateInfo descriptor_layout_create_info = { 0 };
    descriptor_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_layout_create_info.pNext = NULL;
    descriptor_layout_create_info.flags = 0;
    descriptor_layout_create_info.bindingCount = 1;
    descriptor_layout_create_info.pBindings = &descriptor_binding;

    /* Create this pipelines descriptor set layout */
    VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
    vkCreateDescriptorSetLayout(vulkan_data->device, &descriptor_layout_create_info, NULL, &descriptor_set_layout);

    /* Update the new descriptor set info */
    VkDescriptorSetAllocateInfo descriptor_set_allocate_info = { 0 };
    descriptor_set_allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptor_set_allocate_info.pNext = NULL;
    descriptor_set_allocate_info.descriptorPool = vulkan_data->descriptor_pool;
    descriptor_set_allocate_info.descriptorSetCount = 1;
    descriptor_set_allocate_info.pSetLayouts = &descriptor_set_layout;
    
    /* Allocate the descriptor sets */
    VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
    VK_CHECK(vkAllocateDescriptorSets(vulkan_data->device, &descriptor_set_allocate_info, &descriptor_set), "Could not allocate a descriptor set!");

    /* Build the write descriptors buffer info */
    VkDescriptorBufferInfo buffer_info = { 0 };
    buffer_info.buffer = buffer;
    buffer_info.offset = 0;
    buffer_info.range = buffer_size;

    /* Build the descriptor writes */
    VkWriteDescriptorSet write_descriptor = { 0 };
    write_descriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write_descriptor.pNext = NULL;
    write_descriptor.dstSet = descriptor_set;
    write_descriptor.dstBinding = 0;
    write_descriptor.dstArrayElement = 0;
    write_descriptor.descriptorCount = 1;
    write_descriptor.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write_descriptor.pImageInfo = NULL;
    write_descriptor.pBufferInfo = &buffer_info;
    write_descriptor.pTexelBufferView = NULL;

    /* Update the descriptor set */
    vkUpdateDescriptorSets(vulkan_data->device, 1, &write_descriptor, 0, NULL);

    /* Create the odin_vulkan_buffer_uniform */
    odin_vulkan_buffer_uniform* new_uniform = (odin_vulkan_buffer_uniform*)aero_malloc(sizeof(odin_vulkan_buffer_uniform));
    new_uniform->buffer_size = buffer_size;
    new_uniform->buffer = buffer;
    new_uniform->buffer_allocation = buffer_allocation;
    new_uniform->descriptor_binding = descriptor_binding;
    new_uniform->descriptor_layout = descriptor_set_layout;
    new_uniform->descriptor_set = descriptor_set;

    return (odin_buffer_uniform*)new_uniform;
}

/* Destroys a uniform buffer */
void odin_vulkan_uniform_destroy
(odin_data* data, odin_buffer_uniform* uniform)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Get the vulkan uniform */
    odin_vulkan_buffer_uniform* vulkan_uniform = (odin_vulkan_buffer_uniform*)uniform;

    /* Destroy the buffer */
    vmaDestroyBuffer(vulkan_data->memory_allocator, vulkan_uniform->buffer, vulkan_uniform->buffer_allocation);
}

/* Changes the data in the uniform */
void odin_vulkan_uniform_update
(odin_data* data, odin_buffer_uniform* uniform, void* buffer_data)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;

    /* Get the vulkan uniform */
    odin_vulkan_buffer_uniform* vulkan_uniform = (odin_vulkan_buffer_uniform*)uniform;

    /* Upload the new information */
    void* memory_data;
    vmaMapMemory(vulkan_data->memory_allocator, vulkan_uniform->buffer_allocation, &memory_data);
        aero_memcpy(memory_data, vulkan_uniform->buffer_size, buffer_data, vulkan_uniform->buffer_size);
    vmaUnmapMemory(vulkan_data->memory_allocator, vulkan_uniform->buffer_allocation);
}