#ifndef __ODIN_PHYSICAL_DEVICE__
#define __ODIN_PHYSICAL_DEVICE__


/* Define Handles */
typedef struct odin_physical_device_t* odin_physical_device;


/** \brief Gets the graphics cards that are on the system. */
void odin_get_physical_devices(odin_render_device data, int *physical_devices_count, odin_physical_device *physical_devices);

/** \brief Gets the properties of a physical device. */
void odin_get_physical_device_properties(odin_render_device data, )


#endif /* __ODIN_PHYSICAL_DEVICE__ */