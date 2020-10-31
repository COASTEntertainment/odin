#ifndef __ODIN_API_H__
#define __ODIN_API_H__


/** \brief All of the supported Render APIs. */
typedef enum odin_api
{
    
    odin_api_noop = 0, /** \brief NOOP, loads functions that do not execute any code but are valid function pointers. */
    odin_api_vulkan = 1 /** \brief Vulkan, loads all vulkan function pointers. */

} odin_api;


/** \brief Loads the function pointers to the respective Render API enum. */
void odin_load_api(odin_api api);


#endif /* __ODIN_API_H__ */