#include "odin/odin.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "aero/a_memory.h"
#include "aero/a_string.h"


void window_processor(odin_window window, odin_event event, odin_event_window_data window_data);

void cube_draw(odin_render_device render_device, odin_draw_data draw_data, odin_render_pass present_pass, odin_framebuffer framebuffer, void* user_data);


typedef struct default_vertex
{

    float position[2];
    float color[3];

} default_vertex_t;


static default_vertex_t verts[4] =
{

    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}

};


static uint32_t indices[6] =
{

    0, 1, 2, 2, 3, 0
};


typedef struct cube_example_data
{

    odin_render_device      render_device;
    odin_vertex_buffer      vertex_buffer;
    odin_index_buffer       index_buffer;
    odin_pipeline           pipeline;

} cube_example_data;

cube_example_data cube_data = { 0 };


int main()
{

    odin_input_device input_device;
    odin_input_device_create(&input_device);

    odin_input_device_set_window_processor(input_device, window_processor);

    odin_api choosen_api = odin_api_vulkan;
    odin_load_api(choosen_api);

    odin_render_device_create_info initialize_info = { 0 };
    initialize_info.application_name = "Cube Example";
    initialize_info.application_version = (odin_version){ 0, 0, 1 };
    initialize_info.engine_name = "Example Engine";
    initialize_info.engine_version = (odin_version){ 0, 0, 1 };


    odin_render_device_create(&cube_data.render_device, &initialize_info);

    /* Create the window. */
    odin_window window = NULL;
    odin_window_create(
        cube_data.render_device, 
        &window, input_device, 
        "Cube Example", 
        0, 0, 800, 600, 
        odin_window_style_defalt);



                          
    /* Get the physical devices. */
    int physical_devices_count = 0;
    odin_get_physical_devices(cube_data.render_device, &physical_devices_count, NULL);

    odin_physical_device* physical_devices = malloc(sizeof(odin_physical_device) * physical_devices_count);
    memset(physical_devices, 0, sizeof(odin_physical_device) * physical_devices_count);

    odin_get_physical_devices(cube_data.render_device, &physical_devices_count, physical_devices);
    odin_set_physical_device(cube_data.render_device, physical_devices[0], window);


    /* Describe the vertex assembly to create vertex buffers. */
    odin_vertex_assembly vertex_assembly;
    odin_vertex_assembly_create(cube_data.render_device, &vertex_assembly, 0, sizeof(default_vertex_t), 2);
    
    odin_vertex_assembly_describe_element(cube_data.render_device, vertex_assembly, 0, odin_vertex_element_format_vec2f);
    odin_vertex_assembly_describe_element(cube_data.render_device, vertex_assembly, 1, odin_vertex_element_format_vec3f);

    odin_vertex_buffer_create(cube_data.render_device, &cube_data.vertex_buffer, vertex_assembly, 4, verts);
    odin_index_buffer_create(cube_data.render_device, &cube_data.index_buffer, 6, indices);


    /* Load the image using stb_image. */
    int texture_width = 0, texture_height = 0, texture_channels = 0;
    stbi_uc* texture_data = stbi_load("default.png", &texture_width, &texture_height, &texture_channels, 4);

    assert(texture_data);

    odin_image texture;
    odin_image_create(cube_data.render_device, &texture, odin_image_format_rgba_8_srgb, texture_width, texture_height, 1, 3, odin_image_samples_1x);
    odin_image_upload_data(cube_data.render_device, texture, texture_data);


    odin_shader_code simple_shader_vert_code;
    odin_shader_code simple_shader_frag_code;

    switch (choosen_api)
    {
    /* For vulkan we must load spirv. */
    case odin_api_vulkan:
        odin_pipeline_shader_load_code(&simple_shader_vert_code, "shaders/vulkan/simple_shader_vert.spv");
        odin_pipeline_shader_load_code(&simple_shader_frag_code, "shaders/vulkan/simple_shader_frag.spv");
        break;
    
    default:
        break;
    }


    odin_shader simple_shader_vert;
    odin_shader simple_shader_frag;
    odin_pipeline_shader_create(cube_data.render_device, &simple_shader_vert, odin_shader_stage_vertex, simple_shader_vert_code);
    odin_pipeline_shader_create(cube_data.render_device, &simple_shader_frag, odin_shader_stage_fragment, simple_shader_frag_code);

    /* WE CAN DRAW!!! */
    odin_render_pass present_pass;
    odin_draw_prepare_window(cube_data.render_device, window, &present_pass);


    odin_pipeline_create(cube_data.render_device, &cube_data.pipeline, simple_shader_vert, simple_shader_frag, present_pass, 1, &vertex_assembly);






    int frame_count = 0;
    clock_t previous_time = clock();

    while(true)
    {
        /* Do input handling. */
        odin_input_poll(input_device);

        /* Draw to the window. */
        odin_draw_frame_window(cube_data.render_device, window, cube_draw, &cube_data);

        

        frame_count++;

        clock_t current_time = clock();
        
        if(current_time - CLOCKS_PER_SEC >= previous_time)
        {
            char buf[32];
            aero_memset(buf, 32, 0);


            aero_strcat(buf, 32, "FPS: ");
            _itoa(frame_count, buf, 10);
            


            ODIN_LOG("cube.c", buf);
            fflush(NULL);

            previous_time = clock();
            frame_count = 0;
        }

    }

    odin_draw_done_window(cube_data.render_device, window);
    


    /* Destroy resources. */
    odin_pipeline_destroy(cube_data.render_device, cube_data.pipeline);
    odin_image_destroy(cube_data.render_device, texture);
    odin_index_buffer_destroy(cube_data.render_device, cube_data.index_buffer);
    odin_vertex_buffer_destroy(cube_data.render_device, cube_data.vertex_buffer);
    odin_vertex_assembly_destroy(cube_data.render_device, vertex_assembly);
    odin_window_destroy(cube_data.render_device, window);


    odin_render_device_destroy(cube_data.render_device);


    odin_input_device_destroy(input_device);
}


void window_processor(odin_window window, odin_event event, odin_event_window_data window_data)
{

    

}


void cube_draw(odin_render_device render_device, odin_draw_data draw_data, odin_render_pass present_pass, odin_framebuffer present_framebuffer, void* user_data)
{

    cube_example_data* draw_cube_data = (cube_example_data*)user_data;

    odin_draw_command_begin_render_pass(render_device, draw_data, present_pass, present_framebuffer);

        odin_draw_command_bind_vertex_buffer(render_device, draw_data, draw_cube_data->vertex_buffer);
        odin_draw_command_bind_index_buffer(render_device, draw_data, draw_cube_data->index_buffer);
        odin_draw_command_bind_pipeline(render_device, draw_data, draw_cube_data->pipeline);
        odin_draw_command_indexed(render_device, draw_data, 6);

    odin_draw_command_end_render_pass(render_device, draw_data, present_pass);

}
