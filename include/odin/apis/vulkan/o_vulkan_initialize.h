#ifndef __ODIN_VULKAN_INITIALIZE__
#define __ODIN_VULKAN_INITIALIZE__


#include "odin/o_data.h"


/** \brief Initializes Vulkan API calls and creates the data variable. */
void odin_vulkan_initialize(odin_data* data);

/** \brief Frees all Vulkan API data and stops all frame rendering processes. */
void odin_vulkan_terminate(odin_data data);


#endif /* __ODIN_VULKAN_INITIALIZE__ */