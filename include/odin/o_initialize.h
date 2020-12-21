#ifndef __ODIN_INITIALIZE_H__
#define __ODIN_INITIALIZE_H__


#include "o_render_device.h"


typedef struct odin_version
{

    int major, minor, patch;

} odin_version;

typedef struct odin_render_device_create_info
{

    const char* application_name; /** \brief The name of the application/main window. */
    odin_version application_version; /** \brief The version of the application */
    const char* engine_name; /** \brief The name of the engine. */
    odin_version engine_version; /** \brief The version of the engine. */

} odin_render_device_create_info;


/** \brief Initializes the odin_data structure and creates any Render API calls that are required. */
void (* odin_render_device_create)(odin_render_device* render_device, odin_render_device_create_info* initialize_info);

/** \brief Stops all Render API resources and frees any required memory. */
void (* odin_render_device_destroy)(odin_render_device render_device);


#endif /* __ODIN_INITIALIZE_H__ */