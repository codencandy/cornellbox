#ifndef CNC_TYPES_H
#define CNC_TYPES_H

#include <simd/simd.h>

typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char   u8;

typedef signed int     s32;
typedef signed short   s16;
typedef signed char     s8;

typedef float          f32;
typedef double         f64;

typedef simd_float2     v2;
typedef simd_float3     v3;

typedef simd_float3x3   m3;
typedef simd_float4x4   m4;

struct VertexInput
{
    v3 m_position;
    v2 m_uv;
};

#endif//CNC_TYPES_H
