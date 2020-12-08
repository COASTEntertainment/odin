#include "odin/apis/vulkan/o_vulkan_api.h"

#include "odin/odin.h"
#include "odin/apis/vulkan/o_vulkan.h"


void odin_vulkan_api_load()
{

    odin_initialize = odin_vulkan_initialize;
    odin_terminate = odin_vulkan_terminate;

    odin_get_monitors = odin_vulkan_get_monitors;
    odin_window_create = odin_vulkan_window_create;
    odin_window_destroy = odin_vulkan_window_destroy;

    odin_get_physical_devices = odin_vulkan_get_physical_devices;
    odin_get_physical_device_properties = odin_vulkan_get_physical_device_properties;
    odin_set_physical_device = odin_vulkan_set_physical_device;

    odin_vertex_assembly_create = odin_vulkan_vertex_assembly_create;
    odin_vertex_assembly_destroy = odin_vulkan_vertex_assembly_destroy;
    odin_vertex_assembly_describe_element = odin_vulkan_vertex_assembly_describe_element;

    odin_vertex_buffer_create = odin_vulkan_vertex_buffer_create;
    odin_vertex_buffer_destroy = odin_vulkan_vertex_buffer_destroy;

    odin_index_buffer_create = odin_vulkan_index_buffer_create;
    odin_index_buffer_destroy = odin_vulkan_index_buffer_destroy;

    odin_pipeline_shader_load_code = odin_vulkan_pipeline_shader_load_code;
    odin_pipeline_shader_create = odin_vulkan_pipeline_shader_create;
    odin_pipeline_create = odin_vulkan_pipeline_create;
    odin_pipeline_destroy = odin_vulkan_pipeline_destroy;

    odin_image_create = odin_vulkan_image_create;
    odin_image_destroy = odin_vulkan_image_destroy;
    odin_image_upload_data = odin_vulkan_image_upload_data;

    odin_framebuffer_create = odin_vulkan_framebuffer_create;
    odin_framebuffer_destroy = odin_vulkan_framebuffer_destroy;

    odin_render_pass_create = odin_vulkan_render_pass_create;
    odin_render_pass_destroy = odin_vulkan_render_pass_destroy;

    odin_draw_prepare_window = odin_vulkan_draw_prepare_window;
    odin_draw_frame_window = odin_vulkan_draw_frame_window;
    odin_draw_done_window = odin_vulkan_draw_done_window;

    odin_draw_command_begin_render_pass = odin_vulkan_draw_command_begin_render_pass;
    odin_draw_command_end_render_pass = odin_vulkan_draw_command_end_render_pass;
    odin_draw_command_bind_index_buffer = odin_vulkan_draw_command_bind_index_buffer;
    odin_draw_command_bind_vertex_buffer = odin_vulkan_draw_command_bind_vertex_buffer;
    odin_draw_command_bind_pipeline = odin_vulkan_draw_command_bind_pipeline;
    odin_draw_command_indexed = odin_vulkan_draw_command_indexed;

}