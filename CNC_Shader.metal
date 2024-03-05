#include <metal_stdlib>

using namespace metal;

struct VertexInput
{
    float3 m_position [[attribute(0)]];
    float2 m_uv       [[attribute(1)]];
};

struct VertexOutput
{
    float4 m_position [[position]];
    float2 m_uv;
};

struct UniformData
{
    float4x4 m_projectionMatrix;
    float2   m_screenSize;
};

vertex VertexOutput VertexShader( const device VertexInput* in      [[buffer(0)]],
                                  constant     UniformData& uniform [[buffer(1)]],
                                  uint vertexId [[vertex_id]] )
{
    VertexOutput out;
    float4 inPos = float4( in[vertexId].m_position, 1.0 );
    out.m_position = uniform.m_projectionMatrix * inPos;
    
    return out;
}

fragment float4 FragmentShader( VertexOutput in [[stage_in]] )
{
    float4 color = { 1.0, 1.0, 0.0, 1.0 };
    return color;
}
