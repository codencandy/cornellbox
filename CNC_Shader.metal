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

vertex VertexOutput VertexShader( VertexInput in [[stage_in]] )
{
    VertexOutput out;
    return out;
}

fragment float4 FragmentShader( VertexOutput in [[stage_in]] )
{
    float4 color;
    return color;
}
