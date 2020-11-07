#ifndef __ODIN_VULKAN_DATA_H__
#define __ODIN_VULKAN_DATA_H__


#include <vulkan/vulkan.h>

#include "odin/o_initialize.h"
#include "odin/o_window.h"

typedef struct odin_vulkan_window_t *odin_vulkan_window;

typedef struct odin_vulkan_render_device_t
{

    odin_initialize_info initialize_info; /** \brief Information passed to the odin_initialize info. */

    VkInstance instance; /** \brief The vulkan instance */
    VkPhysicalDevice physical_device; /** \brief The graphics card to be used. */

    VkQueue graphics_queue; uint32_t graphics_queue_index; /** \brief A queue for creating images on the screen and other basic operations. */
    VkQueue present_queue; uint32_t present_queue_index; /** \brief A queue for putting images on the screen. */

    VkDevice device; /** \brief The vulkan logical device for creating more objects. */

    odin_vulkan_window main_window;

} odin_vulkan_render_device_t, *odin_vulkan_render_device;


#endif /* __ODIN_VULKAN_DATA_H__ */