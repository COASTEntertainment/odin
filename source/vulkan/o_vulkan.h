#ifndef __ENGINE_ODIN_VULKAN_INTERNAL_H__
#define __ENGINE_ODIN_VULKAN_INTERNAL_H__


#include "../odin.h"


#include "Aero/core.h"
#include "Aero/log.h"

#include "vulkan/vulkan.h"

#include "vma.h"

#include "o_vulkan_data.h"
#include "o_vulkan_physical_device.h"
#include "o_vulkan_vertex_assembly.h"
#include "o_vulkan_buffer_vertex.h"
#include "o_vulkan_buffer_index.h"
#include "o_vulkan_buffer_command.h"
#include "o_vulkan_image_2d.h"
#include "o_vulkan_buffer_uniform.h"
#include "o_vulkan_pipeline.h"
#include "o_vulkan_draw.h"
#include "o_vulkan_input.h"
#include "o_vulkan_platform.h"


extern void odin_vulkan_get_api_capabilities                                    
(odin_api_capabilities* capabilities);

extern void odin_vulkan_init                                                    
(odin_data** data);

extern void odin_vulkan_free                                                    
(odin_data* data);


/* Creates a validation debug messenger */
VkResult odin_vulkan_create_debug_utils_messenger
(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* debugMessengerCreateInfo, const VkAllocationCallbacks* allocationCallbacks, VkDebugUtilsMessengerEXT* pMessenger);

/* Destroys a validation debug messenger */
void odin_vulkan_destroy_debug_utils_messenger
(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);

/* Debug callback for validation layers */
VKAPI_ATTR VkBool32 VKAPI_CALL odin_vulkan_validation_callback
(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

#define VK_CHECK(con, message)       \
    if(con != VK_SUCCESS) LOG_ERROR(message);


#endif /* __ENGINE_ODIN_VULKAN_INTERNAL_H__ */