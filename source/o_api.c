#include "odin/o_api.h"


/* Include Render API files */
#include "odin/o_log.h"
#include "odin/apis/noop/o_noop_api.h"
#ifdef ODIN_HAS_VULKAN
    #include "odin/apis/vulkan/o_vulkan_api.h"
#endif 


void odin_load_api(odin_api api)
{

    switch (api)
    {
    case odin_api_noop:
        odin_noop_api_load();
        break;
    
    case odin_api_vulkan:
#ifdef ODIN_HAS_VULKAN
        odin_vulkan_api_load();
#else
        ODIN_ERROR("o_api.c", "API Error: Could not load vulkan as it is not available!");
#endif
        break;


    default:
        break;
    }

}