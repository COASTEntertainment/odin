#ifndef __ODIN_VULKAN_WINDOW_H__
#define __ODIN_VULKAN_WINDOW_H__


#include "odin/o_window.h"

#include <vulkan/vulkan.h>

#include "odin/o_render_device.h"
#include "odin/apis/vulkan/o_vulkan_render_device.h"
#include "odin/apis/vulkan/o_vulkan_render_pass.h"
#include "odin/o_platform_internal.h"


typedef struct odin_vulkan_window
{

    odin_native_window native_window;

    VkSurfaceKHR surface; /** \brief The drawing surface. */
    uint32_t image_count; /** \brief The amount of images when presenting. */
    VkImage* images; /** \brief The presentable images in the swapchain. */
    VkImageView* image_views; /** \brief The image views for the swapchain images. */
    VkFramebuffer* image_framebuffers; /** \brief The framebuffers to present in render passes. */
    VkSurfaceFormatKHR surface_format; /** \brief The format of images being presented. */
    VkExtent2D surface_extent; /** \brief The size of the window. */
    VkPresentModeKHR present_mode; /** \brief How swapchain images should be presented to the screen. */
    VkSwapchainKHR swapchain; /** \brief The swapchain. */

    odin_vulkan_render_pass present_pass; /** \brief The render pass for */
    VkCommandBuffer* command_buffers; /** \brief Command buffer to send commands. */
    VkSemaphore* image_available_semaphores; /** \brief Semaphores to determine when an image is finished. */
    VkSemaphore* render_complete_semaphores; /** \brief Semaphores to determine when an image should be drawn to. */
    VkFence* in_flight_fences; /** \brief Fences to know when an image is in flight. */
    VkFence* images_in_flight; /** \brief Fences for when an image is being used. */

} odin_vulkan_window_t, *odin_vulkan_window;


void odin_vulkan_window_create(odin_render_device render_device, odin_window* window, odin_input_device input_device, const char* title, int x, int y, int width, int height, odin_window_style style);

void odin_vulkan_window_destroy(odin_render_device render_device, odin_window window);

void odin_vulkan_window_fullscreen(odin_render_device render_device, odin_window window, bool fullscreen);

/** \brief Retreives the size of the window. */
void odin_vulkan_window_get_size(
    odin_window window,
    int* width,
    int* height);

/** \brief Changes the size of the window. */
void odin_vulkan_window_resize(
    odin_window window, 
    int width, 
    int height);

/** \brief Creates a swapchain for a window. */
void odin_vulkan_window_swapchain_create(odin_vulkan_render_device render_device, odin_vulkan_window window);

/** \brief Destroys a swapchain for a window. */
void odin_vulkan_window_swapchain_destroy(odin_vulkan_render_device render_device, odin_vulkan_window window);

/** \brief Creates a windows resources like framebuffers and image views. */
void odin_vulkan_window_resources_create(odin_vulkan_render_device render_device, odin_vulkan_window window);

/** \brief Destroys a windows framebuffers and image views. */
void odin_vulkan_window_resources_destroy(odin_vulkan_render_device render_device, odin_vulkan_window window);


#endif /* __ODIN_VULKAN_WINDOW_H__ */