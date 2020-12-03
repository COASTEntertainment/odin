#ifndef __ODIN_FRAMEBUFFER_H__
#define __ODIN_FRAMEBUFFER_H__


#include "o_render_device.h"
#include "o_render_pass.h"
#include "o_image.h"


typedef struct odin_framebuffer *odin_framebuffer;


/* Creates a buffer holding pixel data that can be stored for later usage. */
void ( *odin_framebuffer_create)(odin_render_device render_device, odin_framebuffer* framebuffer, int width, int height, odin_render_pass render_pass, int attachments_count, odin_image* attachments);

/* Destroys a framebuffer. */
void ( *odin_framebuffer_destroy)(odin_render_device render_device, odin_framebuffer framebuffer);


#endif /* __ODIN_FRAMEBUFFER_H__ */