#ifndef __ODIN_VULKAN_PLATFORM_H__
#define __ODIN_VULKAN_PLATFORM_H__


#include <vulkan/vulkan.h>


/** \brief Gets the required validation layers for a platform. */
const char **odin_vulkan_platform_get_validation_layers(uint32_t *validation_layers_count);

/** \brief Gets the required instance extensions for a platform. */
const char **odin_vulkan_platform_get_instance_extensions(uint32_t *instance_extensions_count);

/** \brief Gets the required device extensions for a platform .*/
const char **odin_vulkan_platform_get_device_extensions(uint32_t *device_extensions_count);


#endif /* __ODIN_VULKAN_PLATFORM_H__ */