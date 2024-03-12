#ifndef CNC_TYPES_H
#define CNC_TYPES_H

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 500

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
typedef simd_float4     v4;

typedef simd_float3x3   m3;
typedef simd_float4x4   m4;

struct Quarternion
{
    f32 m_q1;
    f32 m_q2;
    f32 m_q3;
    f32 m_q4;
};

struct VertexInput
{
    v3 m_position;
    v2 m_uv;
};

struct UniformData
{
    m4 m_projectionMatrix;
    v2 m_screenSize;
};

struct Camera
{
    v3          m_position;
    v3          m_direction;
    Quarternion m_rotationQuarternion;
    f32         m_near;
    f32         m_far;
    f32         m_fov;
};

struct Box
{
    VertexInput m_vertices[8];
    u32         m_indices[36]; // 6 sides with 6 vertices each
};

struct DrawCall
{
    VertexInput* m_vertices;
    u32*         m_indices;
    u32          m_numVertices;
    u32          m_numIndices;
};

#endif//CNC_TYPES_H
