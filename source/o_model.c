#include "o_model.h"

#include "o_model_gltf.h"

#include "Aero/a_string.h"
#include "Aero/a_memory.h"

#include "assert.h"
#include "stdbool.h"

odin_model* odin_model_load
(aero_path path, odin_vertex_type vertex_type)
{
    /* Allocate the model */
    odin_model* model = (odin_model*)aero_malloc(sizeof(odin_model));

    /* Make sure this path exists */
    assert(aero_filesystem_path_exists(path) == true);

    /* Used to check file types */
    bool indicator = false;

    /* Check for a gltf file type */
    aero_strcmp(aero_filesystem_path_extension(path), PATH_MAX, ".gltf", (int*)&indicator);
    aero_strcmp(aero_filesystem_path_extension(path), PATH_MAX, ".glb", (int*)&indicator);

    /* If its gltf */
    if(indicator)
    {
        /* Load the gltf model */
        odin_model_gltf_load(path, model, vertex_type);
        return model;
    }

    return NULL;
}