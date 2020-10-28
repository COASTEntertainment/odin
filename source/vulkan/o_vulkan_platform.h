#ifndef __ENGINE_ODIN_PLATFORM_VULKAN_INTERNAL_H__
#define __ENGINE_ODIN_PLATFORM_VULKAN_INTERNAL_H__


#include <vulkan/vulkan.h>

#include "o_vulkan_data.h"
#include "o_vulkan_platform_data.h"


/* Initalizes the platform and creates swapchain */
extern void odin_vulkan_platform_init
(odin_data *data);

/* Frees all platform and swapchain resources */
extern void odin_vulkan_platform_free
(odin_data *data);

/* Gets the window handle from the platform */
extern void *odin_vulkan_platform_get_window_handle
(odin_data *data);

/* Gets the window width */
extern float odin_vulkan_platform_window_get_width
(odin_data *data);

/* Gets the window height */
extern float odin_vulkan_platform_window_get_height
(odin_data *data);



/* Gets the validation layers for a platform */
extern const char **odin_vulkan_platform_get_validation_layers
(uint32_t *count);

/* Gets the required instance extensions for a platform */
extern const char **odin_vulkan_platform_get_instance_extensions
(uint32_t *count);

/* Get the required device extensions for a platform */
extern const char **odin_vulkan_platform_get_device_extensions
(uint32_t *count);

/* Frees all swapchain information and recreates it */
extern void odin_vulkan_platform_recreate_swapchain
(odin_vulkan_data *vulkan_data);

/* Destroys all swapchain information */
extern void odin_vulkan_platform_destroy_swapchain
(odin_vulkan_data *vulkan_data);


#endif /* __ENGINE_ODIN_PLATFORM_VULKAN_INTERNAL_H__ */