#ifndef __ODIN_VULKAN_DATA_H__
#define __ODIN_VULKAN_DATA_H__


#include <vulkan/vulkan.h>

#include "odin/o_initialize.h"
#include "odin/o_window.h"


typedef struct odin_vulkan_render_device_t
{

    odin_initialize_info initialize_info; /** \brief Information passed to the odin_initialize info. */

    VkInstance instance; /** \brief The vulkan instance */

    odin_window main_window;

} odin_vulkan_render_device_t, *odin_vulkan_render_device ;


#endif /* __ODIN_VULKAN_DATA_H__ */