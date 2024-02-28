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
    float4x4 m_projection;
};

vertex VertexOutput VertexShader( VertexInput in [[stage_in]],
                                  constant UniformData& uniform [[buffer(1)]] )
{
    VertexOutput out;
    out.m_position = float4( in.m_position, 1.0 ) * uniform.m_projection;
    return out;
}

fragment float4 FragmentShader( VertexOutput in [[stage_in]] )
{
    float4 color = { 1.0, 1.0, 0.0, 1.0 };
    return color;
}
