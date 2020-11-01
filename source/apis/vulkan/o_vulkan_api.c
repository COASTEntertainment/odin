#include "odin/apis/vulkan/o_vulkan_api.h"

#include "odin/odin.h"
#include "odin/apis/vulkan/o_vulkan.h"


void odin_vulkan_api_load()
{

    odin_initialize = odin_vulkan_initialize;
    odin_terminate = odin_vulkan_terminate;

}