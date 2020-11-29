#ifndef __ODIN_TEXTURE_2D_H__
#define __ODIN_TEXTURE_2D_H__


#include "stdint.h"

#include "o_render_device.h"


typedef struct odin_texture_2d *odin_texture_2d;

typedef enum odin_image_format
{

    /* If you want to use specific types use these */
    /*  
        u = unsigned 
        n = normalized

        odin_texture_2d_image_format_[count]_[bits]_[modifiers]_[type]

    */

    odin_image_format_r_8_u_int           = 0,
    odin_image_format_r_16_u_int          = 1,
    odin_image_format_r_32_u_int          = 2,

    odin_image_format_r_8_int             = 3,
    odin_image_format_r_16_int            = 4,
    odin_image_format_r_32_int            = 5,

    odin_image_format_r_32_float          = 6,

    odin_image_format_rg_8_u_int          = 7,
    odin_image_format_rg_16_u_int         = 8,
    odin_image_format_rg_32_u_int         = 9,

    odin_image_format_rg_16_float         = 10,
    odin_image_format_rg_32_float         = 11,

    odin_image_format_rgb_8_srgb          = 12,

    odin_image_format_rgb_16_float        = 13,
    odin_image_format_rgb_32_float        = 14,

    odin_image_format_rgba_8_srgb         = 15,

    odin_image_format_rgba_16_float       = 16,
    odin_image_format_rgba_32_float       = 17,


    /* Basic info enums to make it easier. */
    odin_image_format_color               = odin_image_format_rgb_8_srgb,
    odin_image_format_color_alpha         = odin_image_format_rgba_8_srgb,
    odin_image_format_normal              = odin_image_format_rgb_32_float,
    odin_image_format_depth               = odin_image_format_rgb_32_float,
    odin_image_format_roughness           = odin_image_format_r_32_float,
    odin_image_format_metallic            = odin_image_format_r_32_float
    
} odin_image_format;

typedef enum odin_texture_2d_samples
{

    odin_texture_2d_samples_1x  = 0,
    odin_texture_2d_samples_2x  = 1,
    odin_texture_2d_samples_4x  = 2,
    odin_texture_2d_samples_8x  = 3,
    odin_texture_2d_samples_16x = 4,
    odin_texture_2d_samples_32x = 5,
    odin_texture_2d_samples_64x = 6  /* To be used later on insane computers of the future. Vulkan supports this. */

} odin_texture_2d_samples;


/** \brief Creates a two dimensional texture that can be used for many things. */
void ( *odin_texture_2d_create)(odin_render_device render_device, odin_texture_2d* texture, odin_image_format format, int width, int height, int mip_levels, odin_texture_2d_samples samples, int32_t size, void *data);

/** \brief Destroys the image object and its memory. */
void ( *odin_texture_2d_destroy)(odin_render_device render_device, odin_texture_2d texture);


#endif /* __ODIN_TEXTURE_2D_H__ */