#ifndef __ODIN_VULKAN_INITIALIZE__
#define __ODIN_VULKAN_INITIALIZE__


#include "odin/o_initialize.h"


/** \brief Initializes Vulkan API calls and creates the render_device handle. */
void odin_vulkan_initialize(odin_render_device *render_device, odin_render_device_create_info* initialize_info);

/** \brief Frees all Vulkan API data and stops all frame rendering processes. */
void odin_vulkan_terminate(odin_render_device render_device);


#endif /* __ODIN_VULKAN_INITIALIZE__ */