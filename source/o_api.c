#include "odin.h"

#include "Aero/log.h"

/* Include all APIS */

#ifdef ODIN_VULKAN
    #include "vulkan/o_vulkan.h"

    void odin_load_api_vulkan();
#endif



void odin_load_api(odin_api api)
{

    switch(api)
    {


        case odin_api_vulkan:
#ifdef ODIN_VULKAN
            odin_load_api_vulkan();
#else
            LOG_ERROR("Vulkan specified but it is not supported!");
#endif
        break;


        default:
            LOG_ERROR("Specified rendering API was invalid!");
        break;

    }

}


#ifdef ODIN_VULKAN
    void odin_load_api_vulkan()
    {

        odin_get_api_capabilities = odin_vulkan_get_api_capabilities;
        odin_init = odin_vulkan_init;
        odin_free = odin_vulkan_free;

        odin_get_physical_devices               = odin_vulkan_get_physical_devices;
        odin_physical_device_get_information    = odin_vulkan_physical_device_get_information;
        odin_physical_device_use                = odin_vulkan_physical_device_use;

        odin_vertex_assembly_create             = odin_vulkan_vertex_assembly_create;
        odin_vertex_assembly_destroy            = odin_vulkan_vertex_assembly_destroy;
        odin_vertex_assembly_add_element        = odin_vulkan_vertex_assembly_add_element;

        odin_buffer_vertex_create               = odin_vulkan_buffer_vertex_create;
        odin_buffer_vertex_destroy              = odin_vulkan_buffer_vertex_destroy;

        odin_buffer_index_create                = odin_vulkan_buffer_index_create;
        odin_buffer_index_destroy               = odin_vulkan_buffer_index_destroy;

        odin_buffer_uniform_create              = odin_vulkan_uniform_create;
        odin_buffer_uniform_destroy             = odin_vulkan_uniform_destroy;
        odin_buffer_uniform_update              = odin_vulkan_uniform_update;

        odin_shader_load                        = odin_vulkan_shader_load;
        odin_shader_vertex_create               = odin_vulkan_shader_vertex_create;
        odin_shader_fragment_create             = odin_vulkan_shader_fragment_create;
        odin_pipeline_create                    = odin_vulkan_pipeline_create;
        odin_pipeline_destroy                   = odin_vulkan_pipeline_destroy;

        odin_draw_set_function                  = odin_vulkan_draw_set_function;
        odin_draw_prepare                       = odin_vulkan_draw_prepare;
        odin_draw_done                          = odin_vulkan_draw_done;
        odin_draw_set_buffer_vertex             = odin_vulkan_draw_set_buffer_vertex;
        odin_draw_set_buffer_index              = odin_vulkan_draw_set_buffer_index;
        odin_draw_set_pipeline                  = odin_vulkan_draw_set_pipeline;
        odin_draw_indexed                       = odin_vulkan_draw_indexed;

        odin_draw_frame                         = odin_vulkan_draw_frame;

        odin_input_on_window_resize             = odin_vulkan_input_on_window_resize;

        odin_platform_get_window_handle         = odin_vulkan_platform_get_window_handle;
        odin_platform_window_get_width          = odin_vulkan_platform_window_get_width;
        odin_platform_window_get_height         = odin_vulkan_platform_window_get_height;
    }
#endif