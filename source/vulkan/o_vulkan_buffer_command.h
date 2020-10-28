#ifndef __ODIN_VULKAN_BUFFER_COMMAND_H__
#define __ODIN_VULKAN_BUFFER_COMMAND_H__


#include "../o_buffer_command.h"

#include <vulkan/vulkan.h>

#include "o_vulkan_data.h"


typedef struct odin_vulkan_buffer_command
{
    VkCommandBuffer buffer;
} odin_vulkan_buffer_command;


/* Single time commands for any task that can run in multiple threads */
VkCommandBuffer odin_vulkan_specific_begin_single_time_commands
(odin_vulkan_data* data);

/* Ends single time commands */
void odin_vulkan_specific_end_single_time_commands
(odin_vulkan_data* data, VkCommandBuffer command_buffer);


#endif /* __ODIN_VULKAN_BUFFER_COMMAND_H__ */