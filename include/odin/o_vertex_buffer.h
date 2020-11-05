#ifndef __ODIN_VERTEX_BUFFER_H__
#define __ODIN_VERTEX_BUFFER_H__


#include "odin/o_render_device.h"


typedef struct odin_vertex_buffer_t *odin_vertex_buffer;


void odin_vertex_buffer_create(odin_render_device render_device, odin_vertex_buffer* vertex_buffer,  );


#endif /* __ODIN_VERTEX_BUFFER_H__ */