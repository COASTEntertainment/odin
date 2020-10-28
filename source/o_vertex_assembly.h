#ifndef __ODIN_VERTEX_ASSEMBLY_H__
#define __ODIN_VERTEX_ASSEMBLY_H__


#include "stdint.h"

#include "o_data.h"


/* Different types of information that can be passed with a vertex element */
typedef enum odin_vertex_element_type
{

    odin_vertex_element_type_int                = 0,
    odin_vertex_element_type_unsigned_int       = 1,
    odin_vertex_element_type_normalized_int     = 2,
    odin_vertex_element_type_float              = 3,
    odin_vertex_element_type_vec2i              = 4,
    odin_vertex_element_type_vec2f              = 5,
    odin_vertex_element_type_vec3i              = 6,
    odin_vertex_element_type_vec3f              = 7,
    odin_vertex_element_type_vec4i              = 8,
    odin_vertex_element_type_vec4f              = 9

} odin_vertex_element_type; 

/* How a vertex should be sent to a shader */
typedef struct odin_vertex_assembly odin_vertex_assembly; 


/* Creates a vertex assembly */
odin_vertex_assembly* (* odin_vertex_assembly_create)
(odin_data* data, int32_t vertex_size);

/* Destroys a vertex assembly because it will not be used anymore */
void (* odin_vertex_assembly_destroy)
(odin_data* data, odin_vertex_assembly* vertex_assembly); 

 /* Adds an element to a vertex assembly */
void (* odin_vertex_assembly_add_element)
(odin_data* data, odin_vertex_assembly* vertex_assembly, odin_vertex_element_type element_type, int32_t offset);


#endif /* __ODIN_VERTEX_ASSEMBLY_H__ */