#ifndef __ODIN_VULKAN_INPUT_H__
#define __ODIN_VULKAN_INPUT_H__


#include "o_vulkan_data.h"




/* When a window is resized this should be called */
extern void odin_vulkan_input_on_window_resize
(odin_data* data);

/* Gets the windows width */
extern float odin_vulkan_input_get_window_width
(odin_data* data);

/* Gets the windows height */
extern float odin_vulkan_input_get_window_height
(odin_data* data);



#endif /* __ODIN_VULKAN_INPUT_H__ */