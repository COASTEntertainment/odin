#ifndef __ODIN_DRAW_DATA_H__
#define __ODIN_DRAW_DATA_H__


#include "o_render_device.h"


/* Info such as command buffers or general drawing data. */
typedef struct odin_draw_data_t *odin_draw_data;


typedef void( *odin_draw_func)(odin_render_device render_device, odin_draw_data draw_data);


#endif /* __ODIN_DRAW_DATA_H__ */