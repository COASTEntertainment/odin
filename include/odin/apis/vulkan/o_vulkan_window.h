#ifndef __ODIN_VULKAN_WINDOW_H__
#define __ODIN_VULKAN_WINDOW_H__


#include "odin/o_window.h"

#include <vulkan/vulkan.h>

#include "odin/o_render_device.h"
#include "odin/apis/vulkan/o_vulkan_render_device.h"


/** \brief A base vulkan abstraction for a window/drawing surface. */
typedef struct odin_vulkan_window_t
{

    VkSurfaceKHR surface; /** \brief The drawing surface. */
    uint32_t image_count; /** \brief The amount of images when presenting. */
    VkSurfaceFormatKHR surface_format; /** \brief The format of images being presented. */
    VkExtent2D surface_extent; /** \brief The size of the window. */
    VkPresentModeKHR present_mode; /** \brief How swapchain images should be presented to the screen. */
    VkSwapchainKHR swapchain; /** \brief The swapchain. */
    

} odin_vulkan_window_t, *odin_vulkan_window;


/** \brief Gets a systems monitors and their information. */
void odin_vulkan_get_monitors(odin_render_device render_device, int *monitors_count, odin_monitor *monitors);

/** \brief Creates a window for rendering to. */
void odin_vulkan_window_create(odin_render_device render_device, odin_window* window, const char* title, int x, int y, int width, int height, odin_window_style style, bool fullscreen, odin_monitor monitor);

/** \brief Destroys a window. */
void odin_vulkan_window_destroy(odin_render_device render_device, odin_window window);

/** \brief Changes the windows to be fullscreen or not. */
void odin_vulkan_window_fullscreen(odin_render_device render_device, odin_window window, bool fullscreen);


/** \brief Creates a swapchain for a window. */
void odin_vulkan_window_swapchain_create(odin_vulkan_render_device render_device, odin_vulkan_window window);

/** \brief Destroys a swapchain for a window. */
void odin_vulkan_window_swapchain_destroy(odin_vulkan_render_device render_device, odin_vulkan_window window);


#endif /* __ODIN_VULKAN_WINDOW_H__ */