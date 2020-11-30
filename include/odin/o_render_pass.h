#ifndef __ODIN_RENDER_PASS_H__
#define __ODIN_RENDER_PASS_H__


#include "o_render_pass.h"
#include "o_render_device.h"


typedef struct odin_render_pass_t *odin_render_pass;


/** \brief Creates a render pass. */
void odin_render_pass_create(odin_render_device render_device, odin_render_pass* render_pass);

/** \brief Destroys a render pass. */
void odin_render_pass_destroy(odin_render_device render_device, odin_render_pass render_pass);



#endif /* __ODIN_RENDER_PASS_H__ */