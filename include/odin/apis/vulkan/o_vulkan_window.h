#ifndef __ODIN_VULKAN_WINDOW_H__
#define __ODIN_VULKAN_WINDOW_H__


#include "odin/o_window.h"
#include "odin/o_render_device.h"


/** \brief Gets a systems monitors and their information. */
void odin_vulkan_get_monitors(odin_render_device render_device, int *monitors_count, odin_monitor *monitors);

/** \brief Creates a window for rendering to. */
void odin_vulkan_window_create(odin_render_device render_device, odin_window* window, const char* title, int x, int y, int width, int height, odin_window_style style, bool fullscreen, odin_monitor monitor);

/** \brief Destroys a window. */
void odin_vulkan_window_destroy(odin_render_device render_device, odin_window window);

/** \brief Changes the windows to be fullscreen or not. */
void odin_vulkan_window_fullscreen(odin_render_device render_device, odin_window window, bool fullscreen);


#endif /* __ODIN_VULKAN_WINDOW_H__ */