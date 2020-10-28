#ifndef __ODIN_API_H__
#define __ODIN_API_H__


/* Allows for choosing a specific API to load */
typedef enum odin_api
{

    odin_api_vulkan

} odin_api; 

/* Capabilities of specific APIs */
typedef struct odin_api_capabilities
{

    bool physical_devices;

} odin_api_capabilities; 


/* Loads a specific render api into the function pointers */
extern void odin_api_load
(odin_api api); 

/* Gets the api capabilities from the api */
void (* odin_api_get_capabilities)
(odin_api_capabilities* capabilities);


#endif /* __ODIN_API_H__ */