#ifndef __ODIN_VERTEX_ASSEMBLY_H__
#define __ODIN_VERTEX_ASSEMBLY_H__


#include <stdint.h>


/** \brief Describes what a vertex should look like for shaders and GPU transfers. */
typedef struct odin_vertex_assembly_t *odin_vertex_assembly;


typedef enum odin_vertex_element_format
{

    odin_vertex_element_format_int32 = 0,
    odin_vertex_element_format_int64 = 1,
    odin_vertex_element_format_float = 2,
    odin_vertex_element_format_vec2f = 3,
    odin_vertex_element_format_vec3f = 4,
    odin_vertex_element_format_vec4f = 5,
    odin_vertex_element_format_vec2i32 = 6,
    odin_vertex_element_format_vec3i32 = 7,
    odin_vertex_element_format_vec4i32 = 8

} odin_vertex_element_format;


/** \brief Creates a vertex assembly. */
void ( *odin_vertex_assembly_create)(odin_render_device render_device, odin_vertex_assembly* vertex_assembly, int binding, int32_t vertex_size, int elements_count);

/** \brief Destroys a vertex assembly. */
void ( *odin_vertex_assembly_destroy)(odin_render_device render_device, odin_vertex_assembly vertex_assembly);

/** \brief Adds an element to a vertex assembly for description. */
void ( *odin_vertex_assembly_describe_element)(odin_render_device render_device, odin_vertex_assembly vertex_assembly, int element_index, odin_vertex_element_format format);


#endif /* __ODIN_VERTEX_ASSEMBLY_H__ */