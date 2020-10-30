#ifndef __ENGINE_ODIN_VULKAN_INTERNAL_H__
#define __ENGINE_ODIN_VULKAN_INTERNAL_H__


#include "../odin.h"
#include "../odin_config.h"
#include "../o_api.h"

#include "vulkan/vulkan.h"
#include "vma.h"

#include "o_vulkan_data.h"
#include "o_vulkan_physical_device.h"
#include "o_vulkan_vertex_assembly.h"
#include "o_vulkan_vertex_buffer.h"
#include "o_vulkan_index_buffer.h"
#include "o_vulkan_uniform_buffer.h"
#include "o_vulkan_image_2d.h"
#include "o_vulkan_pipeline.h"
#include "o_vulkan_draw.h"
#include "o_vulkan_input.h"
#include "o_vulkan_platform.h"


extern void odin_vulkan_get_api_capabilities                                    
(odin_api_capabilities* capabilities);

extern void odin_vulkan_init                                                    
(odin_data** data, odin_initalization_information* initalization_info);

extern void odin_vulkan_free                                                    
(odin_data* data);



#define VK_CHECK(con, filename, id, message)       \
    if(con != VK_SUCCESS) ODIN_ERROR(filename, __LINE__, id, message);


#endif /* __ENGINE_ODIN_VULKAN_INTERNAL_H__ */