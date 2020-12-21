#ifndef __ODIN_WINDOW_H__
#define __ODIN_WINDOW_H__


#include <stdbool.h>

#include "odin/o_render_device.h"


/* Forward Dec */
typedef struct odin_input_device *odin_input_device;

/** \brief A interface for an API that can use a windowing system. */
typedef struct odin_window *odin_window;

/** \brief How a window is supposed to act in an operating system */
typedef enum odin_window_style
{

    odin_window_style_defalt = 0, /** \brief A window with average buttons on every window. */
    odin_window_style_minimal = 1 /** \brief Creates a window with no title bar or buttons. */

} odin_window_style;

/** \brief A representation of a monitor and its settings. */
typedef struct odin_monitor_t
{
    
    int width;
    int height;
    int refresh_rate;

} odin_monitor_t, *odin_monitor;


/** \brief Gets a systems monitors and their information. */
void ( *odin_get_monitors)(odin_render_device render_device, int *monitors_count, odin_monitor *monitors);

/** \brief Creates a window for rendering to. */
void ( *odin_window_create)(odin_render_device render_device, odin_window *window, odin_input_device input_device, const char *title, int x, int y, int width, int height, odin_window_style style, bool fullscreen, odin_monitor monitor);

/** \brief Destroys a window. */
void ( *odin_window_destroy)(odin_render_device render_device, odin_window window);

/** \brief Changes the windows to be fullscreen or not. */
void ( *odin_window_fullscreen)(odin_render_device render_device, odin_window window, bool fullscreen, odin_monitor monitor);

/** \brief Retrieves the input_device pointer from the window. */
void ( *odin_window_get_input_device)(odin_window window, odin_input_device* input_device);


#endif /* __ODIN_WINDOW_H__ */