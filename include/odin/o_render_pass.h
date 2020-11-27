#ifndef __ODIN_RENDER_PASS_H__
#define __ODIN_RENDER_PASS_H__


#include "o_render_pass.h"
#include "o_render_device.h"


typedef struct odin_render_pass_t *odin_render_pass;


/** \brief Creates a render pass. */
void odin_render_pass_create();

/** \brief Destroys a render pass. */
void odin_render_pass_destroy();



#endif /* __ODIN_RENDER_PASS_H__ */