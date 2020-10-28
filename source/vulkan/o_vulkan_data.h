#ifndef __ODIN_VULKAN_DATA_H__
#define __ODIN_VULKAN_DATA_H__



#include "stdbool.h"

#include "../odin_config.h"
#include "../o_data.h"
#include "../o_draw.h"
#include "../o_draw_data.h"
#include "o_vulkan_platform_data.h"

#include <vulkan/vulkan.h>
#include "vma.h"


typedef struct odin_vulkan_data
{
    
    bool                        validation_enabled;
    VkInstance                  instance;
    VkDebugUtilsMessengerEXT    debug_utils_messenger;
    VkPhysicalDevice            physical_device;
    VkDevice                    device;

    VkQueue                     graphics_queue;
    uint32_t                    graphics_queue_index;
    VkQueue                     present_queue;
    uint32_t                    present_queue_index;

    odin_vulkan_platform_data*  platform_data;

    VmaAllocator                memory_allocator;
    VkCommandPool               command_pool;
    VkDescriptorPool            descriptor_pool;
    VkRenderPass                render_pass;

    VkSemaphore*                image_available_semaphores;
    VkSemaphore*                render_finished_semaphores;
    VkFence*                    in_flight_fences;
    VkFence*                    images_in_flight;

    ODIN_VECTOR4F               clear_color;
    odin_draw_func              draw_func;

} odin_vulkan_data;


#endif /* __ODIN_VULKAN_DATA_H__ */