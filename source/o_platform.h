#ifndef __ODIN_PLATFORM_H__
#define __ODIN_PLATFORM_H__


#include "o_data.h"


/* Initalizes any platform specific resources */
extern void (* odin_platform_init)
(odin_data* data);

/* Frees any platform specific resources */
extern void (* odin_platform_free)
(odin_data* data);

/* Returns the window handle for any input API 
   When using windows you must set the window procedure yourself! */
extern void* (* odin_platform_get_window_handle)
(odin_data* data);

/* Gets the window width */
extern float (* odin_platform_window_get_width)
(odin_data* data);

/* Gets the window height */
extern float (* odin_platform_window_get_height)
(odin_data* data);


#define ODIN_PLATFORM_DEFAULT_WINDOW_WIDTH 1080
#define ODIN_PLATFORM_DEFAULT_WINDOW_HEIGHT 720


#endif /* __ODIN_PLATFORM_H__ */