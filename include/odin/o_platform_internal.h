#ifndef __ODIN_PLATFORM_INTERNAL_H__
#define __ODIN_PLATFORM_INTERNAL_H__


#include "odin/o_input.h"


#ifdef ODIN_HAS_VULKAN

    #include <vulkan/vulkan.h>

#endif


typedef struct odin_native_window *odin_native_window;
typedef struct odin_window *odin_window;


void odin_platform_init();

void odin_platform_create_native_window(
    odin_native_window*     native_window, 
    odin_window             window,
    odin_input_device	    input_device,
    const char*             title, 
    int                     width, 
    int                     height);

void odin_platform_destroy_native_window(
    odin_native_window         native_window);

void odin_platform_get_native_window_size(
    odin_native_window          native_window,
    int*                        width,
    int*                        height);


#ifdef ODIN_HAS_VULKAN

void odin_platform_create_vulkan_surface(
    odin_native_window      native_window,
    VkInstance              instance,
    VkSurfaceKHR*           surface);

#endif


#endif /* __ODIN_PLATFORM_INTERNAL_H__ */