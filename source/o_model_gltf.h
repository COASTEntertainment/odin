#ifndef __ODIN_MESH_GLTF_H__
#define __ODIN_MESH_GLTF_H__


#include "Aero/a_filesystem.h"

#include "o_mesh.h"


typedef struct odin_model odin_model;


void odin_model_gltf_load
(aero_path path, odin_model* model, odin_vertex_type vertex_type);


#endif /* __ODIN_MESH_GLTF_H__ */