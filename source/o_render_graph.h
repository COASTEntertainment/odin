#ifndef __ODIN_RENDER_GRAPH_H__
#define __ODIN_RENDER_GRAPH_H__


#include "o_handle.h"
#include "o_data.h"


/* Define the render graph handle */
ODIN_DEFINE_HANDLE(odin_render_graph)

void odin_render_graph_create(odin_data data, odin_render_graph* render_graph);
void odin_render_graph_destroy(odin_data data, odin_render_graph render_graph);

void odin_render_graph_add_pass(odin_data data, odin_render_graph render_graph);
void odin_render_graph_
void odin_render_graph_build(odin_data data, odin_render_graph render_graph);

void odin_render_graph_execute(odin_data data, odin_render_graph render_graph);


#endif /* __ODIN_RENDER_GRAPH_H__ */