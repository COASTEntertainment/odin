#ifndef __ODIN_IMAGE_2D_H__
#define __ODIN_IMAGE_2D_H__


#include "o_handle.h"
#include "o_data.h"


/* How an image should be tiled */
typedef enum odin_image_2d_tiling
{
    odin_image_2d_tiling_suggested, /* The optimized tiling mode */
    odin_image_2d_tiling_linear /* A linear tiling mode */
} odin_image_2d_tiling;

/* A two dimensional image that can be used as a texture */
ODIN_DEFINE_HANDLE(odin_image_2d);


/* Creates an image with data */ 
extern odin_image_2d* (* odin_image_2d_create)
(odin_data* data, int width, int height, odin_image_2d_tiling tiling_mode, void* image_data);

/* Creates an image from a file */
extern odin_image_2d* (* odin_image_2d_create_and_load)
(odin_data* data, const char* path);

/* Destroys an image and its memory */
extern void (* odin_image_2d_destroy)
(odin_data* data, odin_image_2d* image);


#endif /* __ODIN_IMAGE_2D_H__ */