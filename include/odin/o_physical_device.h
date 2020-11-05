#ifndef __ODIN_PHYSICAL_DEVICE__
#define __ODIN_PHYSICAL_DEVICE__


#include "odin/o_render_device.h"


/** \brief A handle to data that stores information about a graphics card */
typedef struct odin_physical_device_t* odin_physical_device;


/** \brief Stores information about a physical device that might be required */
typedef struct odin_physical_device_information_t
{

    char name[256]; /** \brief The name of the physical device provided by the Render API */

} odin_physical_device_information_t;


/** \brief Gets the graphics cards that are on the system. */
void ( *odin_get_physical_devices)(odin_render_device render_device, int *physical_devices_count, odin_physical_device *physical_devices);

/** \brief Gets the properties of a physical device. */
void ( *odin_get_physical_device_properties)(odin_render_device render_device, odin_physical_device physical_device, odin_physical_device_information_t* properties);

/** \brief Sets a physical device to be used at runtime when rendering */
void ( *odin_set_physical_device)(odin_render_device render_device, odin_physical_device physical_device, odin_window window);


#endif /* __ODIN_PHYSICAL_DEVICE__ */