#ifndef __ODIN_MODEL_H__
#define __ODIN_MODEL_H__




#include "o_mesh.h"

/* Stores data about bones */
typedef struct odin_bone
{
    int index;
    mat4f transform;
} odin_bone;

/* A mesh that stores meshes and materials */
typedef struct odin_model
{
    int mesh_count;
    odin_mesh* mesh_data;
    
    int bone_count;
    odin_bone* bone_data;
} odin_model;


odin_model* odin_model_load
(const char* path, odin_vertex_type vertex_type);


#endif /* __ODIN_MODEL_H__ */