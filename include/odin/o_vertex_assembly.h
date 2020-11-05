#ifndef __ODIN_VERTEX_ASSEMBLY_H__
#define __ODIN_VERTEX_ASSEMBLY_H__


/** \brief Describes what a vertex should look like for shaders and GPU transfers. */
typedef struct odin_vertex_assembly_t *odin_vertex_assembly;


/** \brief Creates a vertex assembly. */
void odin_vertex_assembly_create(odin_render_device render_device, odin_vertex_assembly* vertex_assembly, int32_t vertex_size, int32_t elements_count);

/** \brief Destroys a vertex assembly. */
void odin_vertex_assembly_destroy(odin_render_device render_device, odin_vertex_assembly vertex_assembly);

/** \brief Adds an element to a vertex assembly for description. */
void odin_vertex_assembly_describe_element(odin_render_device render_device, odin_vertex_assembly vertex_assembly, int element_index, int32_t element_size);


#endif /* __ODIN_VERTEX_ASSEMBLY_H__ */