#ifndef __ODIN_INPUT_H__
#define __ODIN_INPUT_H__


#include "o_data.h"


/* When a window is resized this should be called */
extern void (* odin_input_on_window_resize)
(odin_data* data);

/* Gets the width of the window */
extern float (* odin_input_get_window_width)
(odin_data* data);

/* Gets the height of a window */
extern float (* odin_input_get_window_height)
(odin_data* data);


#endif /* __ODIN_INPUT_N__ */