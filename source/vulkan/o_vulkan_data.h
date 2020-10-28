#ifndef __ODIN_VULKAN_DATA_H__
#define __ODIN_VULKAN_DATA_H__



#include "stdbool.h"

#include "../o_data.h"
#include "../odin_internal.h"

#include <vulkan/vulkan.h>
#include "vma.h"


typedef struct odin_renderpass              odin_renderpass;
typedef struct odin_buffer_frame            odin_buffer_frame;
typedef struct odin_buffer_command          odin_buffer_command;
typedef struct odin_platform_data           odin_platform_data;

typedef struct odin_vulkan_renderpass       odin_vulkan_renderpass;
typedef struct odin_vulkan_framebuffer      odin_vulkan_framebuffer;
typedef struct odin_vulkan_buffer_command   odin_vulkan_buffer_command;
typedef struct odin_vulkan_platform_data    odin_vulkan_platform_data;

typedef struct odin_vulkan_data
{
    bool validation_enabled;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_utils_messenger;
    VkPhysicalDevice physical_device;
    VkDevice device;
    VkQueue graphics_queue;
    uint32_t graphics_queue_index;
    VkQueue present_queue;
    uint32_t present_queue_index;
    VmaAllocator memory_allocator;
    VkCommandPool command_pool;
    VkDescriptorPool descriptor_pool;
    VkRenderPass render_pass;
    
    odin_vulkan_platform_data* platform_data;

    /* Swapchain */
    bool swapchain_out_of_date;
    uint32_t image_count;
    VkPresentModeKHR present_mode;

    /* Defaults */
    odin_vulkan_framebuffer** framebuffers;
    odin_vulkan_buffer_command* command_buffers;

    /* Syncronization */
    VkSemaphore* image_available_semaphores;
    VkSemaphore* render_finished_semaphores;
    VkFence* in_flight_fences;
    VkFence* images_in_flight;

    ODIN_VECTOR4F clear_color;
    void(* odin_current_draw_func)(odin_data* data, odin_buffer_command* command_buffer);

} odin_vulkan_data;


#endif /* __ODIN_VULKAN_DATA_H__ */