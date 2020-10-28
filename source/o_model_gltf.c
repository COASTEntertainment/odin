#include "o_model_gltf.h"

#include "o_model.h"

#include "Aero/a_memory.h"
#include "Aero/a_string.h"

#include "helpers/cgltf.h"


void odin_model_gltf_load
(aero_path path, odin_model* model, odin_vertex_type vertex_type)
{

    /* Build the parsing data */
    cgltf_options options = { 0 };
    cgltf_data* data;

    /* Parse the file */
    cgltf_parse_file(&options, path, &data);
    
    /* Load the buffers */
    cgltf_load_buffers(&options, data, path);

    /* Verify the data */
    cgltf_validate(data);

    /* Get the mesh count and initalize mesh array */
    model->mesh_count = (int)data->meshes_count;
    model->mesh_data = (odin_mesh*)aero_malloc(sizeof(odin_mesh) * model->mesh_count);
    aero_memset(model->mesh_data, sizeof(odin_mesh) * model->mesh_count, 0);

    /* Copy the mesh data */
    for(int i = 0; i < data->meshes_count; i++)
    {
        aero_strcpy(model->mesh_data[i].name, 128, data->meshes[i].name);

        
        
        data->meshes[i].primitives_count;
    }


}