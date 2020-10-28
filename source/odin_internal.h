#ifndef __ODIN_INTERNAL_H__
#define __ODIN_INTERNAL_H__


#include <aether/aether.h>

/* Define the math functions and types */
/* If you need to change these definitions for your project you should */
#define ODIN_VECTOR2I vec2i
#define ODIN_VEC2I_ADD(left, right) vec2f_add(left, right)
#define ODIN_VEC2I_SUB(left, right) vec2f_sub(left, right)
#define ODIN_VEC2I_MUL(left, right) vec2f_mul(left, right)
#define ODIN_VEC2I_DIV(left, right) vec2f_div(left, right)

#define ODIN_VECTOR3I vec3i
#define ODIN_VEC3I_ADD(left, right) vec3i_add(left, right)
#define ODIN_VEC3I_SUB(left, right) vec3i_sub(left, right)
#define ODIN_VEC3I_MUL(left, right) vec3i_mul(left, right)
#define ODIN_VEC3I_DIV(left, right) vec3i_div(left, right)

#define ODIN_VECTOR4I vec4i
#define ODIN_VEC4I_ADD(left, right) vec4i_add(left, right)
#define ODIN_VEC4I_SUB(left, right) vec4i_sub(left, right)
#define ODIN_VEC4I_MUL(left, right) vec4i_mul(left, right)
#define ODIN_VEC4I_DIV(left, right) vec4i_div(left, right)


#define ODIN_VECTOR2F vec2f
#define ODIN_VEC2F_ADD(left, right) vec2f_add(left, right)
#define ODIN_VEC2F_SUB(left, right) vec2f_sub(left, right)
#define ODIN_VEC2F_MUL(left, right) vec2f_mul(left, right)
#define ODIN_VEC2F_DIV(left, right) vec2f_div(left, right)

#define ODIN_VECTOR3F vec3f
#define ODIN_VEC3F_ADD(left, right) vec3f_add(left, right)
#define ODIN_VEC3F_SUB(left, right) vec3f_sub(left, right)
#define ODIN_VEC3F_MUL(left, right) vec3f_mul(left, right)
#define ODIN_VEC3F_DIV(left, right) vec3f_div(left, right)

#define ODIN_VECTOR4F vec4f
#define ODIN_VEC4F_ADD(left, right) vec4f_add(left, right)
#define ODIN_VEC4F_SUB(left, right) vec4f_sub(left, right)
#define ODIN_VEC4F_MUL(left, right) vec4f_mul(left, right)
#define ODIN_VEC4F_DIV(left, right) vec4f_div(left, right)


#define ODIN_MAT3F mat3f
#define ODIN_MAT3F_ADD(left, right) mat3f_add(left, right)
#define ODIN_MAT3F_SUB(left, right) mat3f_sub(left, right)
#define ODIN_MAT3F_MUL(left, right) mat3f_mul(left, right)

#define ODIN_MAT4F mat4f
#define ODIN_MAT4F_ADD(left, right) mat4f_add(left, right)
#define ODIN_MAT4F_SUB(left, right) mat4f_sub(left, right)
#define ODIN_MAT4F_MUL(left, right) mat4f_mul(left, right)


#endif /* __ODIN_INTERNAL_H__ */