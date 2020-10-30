#ifndef __ODIN_PHYSICAL_DEVICE_H__
#define __ODIN_PHYSICAL_DEVICE_H__


#include "o_handle.h"
#include "o_data.h"


/* A representation of a systems actual GPU. These are unique ids and should be same with every run */
ODIN_DEFINE_HANDLE(odin_physical_device);

/* Information about a GPU that is likely to be used */
typedef struct odin_physical_device_information
{

    char name[256];
    
} odin_physical_device_information; 


/* Gets a list of physical device handles */
void (* odin_get_physical_devices)
(odin_data* data, int* devices_count, odin_physical_device** physical_devices); 

/* Gets possibly required physical device info */
void (* odin_physical_device_get_information)
(odin_data* data, odin_physical_device* physical_device, odin_physical_device_information* physical_device_info); 

/* Selects a physical device to use when rendering */
void (* odin_physical_device_use)
(odin_data* data, odin_physical_device* physical_device); 


#endif /* __ODIN_PHYSICAL_DEVICE_H__ */