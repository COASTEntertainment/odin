#ifndef __ODIN_FRAME_BUFFER_H__
#define __ODIN_FRAME_BUFFER_H__


#include "o_render_device.h"


typedef struct odin_frame_buffer *odin_frame_buffer;


typedef enum odin_frame_buffer_attachment_type
{
    odin_render_pass_type_color,
    odin_render_pass_type_position,
    odin_render_pass_type_depth
    
} odin_render_pass_type;


void odin_frame_buffer_create(odin_render_device render_device, odin_frame_buffer* frame_buffer, odin_frame_buffer_attachment_type attachment_type);


#endif __ODIN_FRAME_BUFFER_H__