#ifndef __ODIN_VULKAN_VERTEX_ASSEMBLY_H__
#define __ODIN_VULKAN_VERTEX_ASSEMBLY_H__


/** \brief Creates a vertex assembly. */
void odin_vertex_assembly_create(odin_render_device render_device, odin_vertex_assembly* vertex_assembly, int32_t vertex_size, int32_t elements_count);

/** \brief Destroys a vertex assembly. */
void odin_vertex_assembly_destroy(odin_render_device render_device, odin_vertex_assembly vertex_assembly);


#endif /* __ODIN_VULKAN_VERTEX_ASSEMBLY_H__ */