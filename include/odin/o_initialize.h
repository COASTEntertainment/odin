#ifndef __ODIN_INITIALIZE_H__
#define __ODIN_INITIALIZE_H__


#include "o_render_device.h"


/** \brief Initializes the odin_data structure and creates any Render API calls that are required. */
void (* odin_initialize)(odin_render_device* render_device);

/** \brief Stops all Render API resources and frees any required memory. */
void (* odin_terminate)(odin_render_device render_device);


#endif /* __ODIN_INITIALIZE_H__ */