#ifndef __ODIN_VULKAN_DRAW_H__
#define __ODIN_VULKAN_DRAW_H__


#include "../o_draw.h"


/* When all required render objects are created we can prepare for rendering */
extern void odin_vulkan_draw_prepare
(odin_data* data); 

/* Changes how rendering happens */
extern void odin_vulkan_draw_set_function
(odin_data* data, void(* odin_draw_func)(odin_data* data, odin_buffer_command* commandBuffer));

/* Sets the clear color */
extern void odin_vulkan_draw_set_clear_color_vec4f        
(odin_data* data, float* color);

/* Binds a pipeline for rendering */
extern void odin_vulkan_draw_set_pipeline
(odin_data* data, odin_buffer_command* commandBuffer, odin_pipeline* pipeline);

/* Specifies which vertex buffer to use this draw */
extern void odin_vulkan_draw_set_buffer_vertex            
(odin_data* data, odin_buffer_command* commandBuffer, odin_buffer_vertex* vertexBuffer); 

/* Specifies which index buffer to use this draw */
extern void odin_vulkan_draw_set_buffer_index             
(odin_data* data, odin_buffer_command* commandBuffer, odin_buffer_index* indexBuffer); 

/*
extern void odin_vulkan_render_upload_uniform               
(odin_data* data, odin_buffer_command commandBuffer, odin_pipeline* pipelineInstance, odin_buffer_uniform* uniform, void* value); /* Upload a uniform to a pipeline instance */

/*
extern void odin_vulkan_render_set_buffer_instance_data     
(odin_data* data, odin_buffer_command commandBuffer, odin_buffer_instance_data* instanceDataBuffer); /* Changes the per instance data, when specified the odin_render_submit command will use instancing */

/*
extern void odin_vulkan_render_set_texture                  
(odin_data* data, odin_buffer_command commandBuffer, odin_pipeline_instance* pipelineInstance, int32_t textureIndex, odin_texture_2d texture); */

/* Submits the frame to drawing on the gpu */
extern void odin_vulkan_draw_indexed                      
(odin_data* data, odin_buffer_command* commandBuffer, uint32_t indexCount); 

/* When rendering is to stop */
extern void odin_vulkan_draw_done                         
(odin_data* data); 

/* Tells the api when to draw a frame */
extern void odin_vulkan_draw_frame
(odin_data* data);


#endif /* __ODIN_VULKAN_DRAW_H__ */