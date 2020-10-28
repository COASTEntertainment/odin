#ifndef __ODIN_MESH_H__
#define __ODIN_MESH_H__


#include <aether/aether.h>


/* This is the standard vertex in odin */
typedef struct odin_vertex
{   
    vec3f position;
    vec3f color;
    vec3f normal;
    vec2f tex_coords;
} odin_vertex;

/* This is the standard vertex in odin for skinned meshes */
typedef struct odin_vertex_skinned
{
    vec3f position;
    vec3f color;
    vec3f normal;
    vec2f tex_coords;

    vec4f bones;        /* Bone indices that this vertex is connected to */
    vec4f weights;      /* Bone weights per each index */
} odin_vertex_skinned;

typedef enum odin_vertex_type
{
    odin_vertex_type_standard,
    odin_vertex_type_skinned
} odin_vertex_type;

/* A mesh that stores vertex data and index data */
typedef struct odin_mesh
{
    char name[128];
    int vertex_count;
    odin_vertex_type vertex_type;
    union vertex_data
    {
        odin_vertex* standard_vertices;
        odin_vertex_skinned* skinned_vertices;
    } vertex_data;
    int index_count;
    uint32_t* indices;

} odin_mesh;


#endif /* __ODIN_MESH_H__ */