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
    float4 m_oldPosition;
    float2 m_uv;
};

struct Quarternion
{
    float m_q0;
    float m_q1;
    float m_q2;
    float m_q3;
};

struct UniformData
{
    float4x4    m_projectionMatrix;
    float4x4    m_viewTransform;
    float4x4    m_scaleTransform;
    Quarternion m_rotationQuarternion;
    Quarternion m_inverseRotation;
    float2      m_screenSize;
};

Quarternion toQuarterion( float radians, float x, float y, float z )
{
    Quarternion q = {};
    q.m_q0 = cos( radians * 0.5f );
    q.m_q1 = x * sin( radians * 0.5f );
    q.m_q2 = y * sin( radians * 0.5f );
    q.m_q3 = z * sin( radians * 0.5f );

    return q;
}

Quarternion multiply( Quarternion a, Quarternion b )
{
    Quarternion result = {};

    result.m_q0 = (a.m_q0 * b.m_q0) - (a.m_q1 * b.m_q1) - (a.m_q2 * b.m_q2) - (a.m_q3 * b.m_q3);
    result.m_q1 = (a.m_q0 * b.m_q1) + (a.m_q1 * b.m_q0) - (a.m_q2 * b.m_q3) + (a.m_q3 * b.m_q2);
    result.m_q2 = (a.m_q0 * b.m_q2) + (a.m_q1 * b.m_q3) + (a.m_q2 * b.m_q0) - (a.m_q3 * b.m_q1);
    result.m_q3 = (a.m_q0 * b.m_q3) - (a.m_q1 * b.m_q2) + (a.m_q2 * b.m_q1) + (a.m_q3 * b.m_q0);

    return result;
}

vertex VertexOutput VertexShader( const device VertexInput* in      [[buffer(0)]],
                                  constant     UniformData& uniform [[buffer(1)]],
                                  uint vertexId [[vertex_id]] )
{
    VertexOutput out;
    float4 inPos = float4( in[vertexId].m_position, 1.0 );
    inPos = inPos * uniform.m_scaleTransform;

    // before transforming the vertices rotate them according to the camera
    Quarternion p = { 0.0, inPos.x, inPos.y, inPos.z };
    Quarternion p1 = multiply( uniform.m_inverseRotation, p );
    Quarternion p2 = multiply( p1, uniform.m_rotationQuarternion );
    
    inPos.x = p2.m_q1;
    inPos.y = p2.m_q2;
    inPos.z = p2.m_q3;

    inPos = uniform.m_viewTransform * inPos;
    
    out.m_position    = uniform.m_projectionMatrix * inPos;
    out.m_oldPosition = inPos;
    
    return out;
}

constant float3 origin = {  0.0,  0.0,  0.0 };
constant float3 pMax   = { 10.0, 10.0, 10.0 };

fragment float4 FragmentShader( VertexOutput in [[stage_in]] )
{
    float  maxLength = distance( pMax, origin );
    float4 color = { 1.0, 1.0, 1.0, 1.0 };
    float3 pos   = in.m_oldPosition.xyz;

    //pos.xyz /= in.m_oldPosition.w;
    color.rgb -= (length( pos ) / maxLength) * 0.8;
    return color;
}
