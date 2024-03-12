#include "CNC_Application.h"
#include "CNC_Types.h"
#include <string.h>

f32 toRadians( f32 degrees )
{
    return (M_PI / 180.0f ) * degrees;
}

Quarternion toQuarterion( f32 radians, f32 x, f32 y, f32 z )
{
    Quarternion q = {};
    q.m_q1 = cosf( radians * 0.5f );
    q.m_q2 = x * sinf( radians * 0.5f );
    q.m_q3 = y * sinf( radians * 0.5f );
    q.m_q4 = z * sinf( radians * 0.5f );

    return q;
}

m4 CreateProjectionMatrix( f32 n   /* near */, 
                           f32 f   /* far  */,
                           f32 w   /* screen width */,
                           f32 h   /* screen height */,
                           f32 fov /* field of view angle in degrees */)
{

    /*
        the matrix we construct solves the following problems:

        - transform according to the screens aspect ration -> h/w
        - transform such that the field of view is accounted for
        - transform such that far and near plane are accounted for

        an excellent explanation about how to construct this matrix can be found here:
        https://www.youtube.com/watch?v=EqNcqBdrNyI

        IMPORTANT: the so called perspective divide does not happen with this
                   matrix multiplication. This is done by the hardware directly after
                   the vertex stage and before the fragment stage
     */
    f32 rad       = toRadians( fov );         // scaling factor for the fov
    f32 fovFactor = 1.0 / tanf( rad * 0.5f ); // inverted to make closer things bigger and vice versa

    f32 a = h/w;
    f32 b = fovFactor;  
    f32 z = f / (f-n);

    v4 row1 = { a*b,   0.0,   0.0,   0.0  }; 
    v4 row2 = { 0.0,     b,   0.0,   0.0  }; 
    v4 row3 = { 0.0,   0.0,     z, -(n*z) };  
    v4 row4 = { 0.0,   0.0,   1.0,   0.0  }; 

    return simd_matrix_from_rows( row1, row2, row3, row4 );
}

void Load( Application* application )
{
    // construct a box => 8 corners
    f32 left   =  -10.0f;
    f32 right  =   10.0f;
    f32 bottom =   -5.0f;
    f32 top    =    5.0f;
    f32 far    =   10.0f;
    f32 near   =  -10.0f;

    /*
          P8 ---- P7
         / |      /|
        P4 ---- P3 | 
        | P5 ---|- P6
        |/      | /
        P1 ---- P2
     */

    v3 P1 = { left,  bottom, near }; // (-10,  0,  0)
    v3 P2 = { right, bottom, near }; // ( 10,  0,  0)
    v3 P3 = { right, top,    near }; // ( 10, 10,  0)
    v3 P4 = { left,  top,    near }; // (-10, 10,  0)

    v3 P5 = { left,  bottom, far };  // (-10,  0, 10)
    v3 P6 = { right, bottom, far };  // ( 10,  0, 10)
    v3 P7 = { right, top,    far };  // ( 10, 10, 10)
    v3 P8 = { left,  top,    far };  // (-10, 10, 10)

    application->m_cornellBox.m_vertices[0].m_position = P1;
    application->m_cornellBox.m_vertices[1].m_position = P2;
    application->m_cornellBox.m_vertices[2].m_position = P3;
    application->m_cornellBox.m_vertices[3].m_position = P4;
    application->m_cornellBox.m_vertices[4].m_position = P5;
    application->m_cornellBox.m_vertices[5].m_position = P6;
    application->m_cornellBox.m_vertices[6].m_position = P7;
    application->m_cornellBox.m_vertices[7].m_position = P8;

    // construct the list of indices
    u32 indices[30] = { 
        4, 5, 6, 6, 7, 4, // back
        0, 1, 5, 5, 4, 0, // bottom
        5, 1, 2, 2, 6, 5, // right
        0, 4, 7, 7, 3, 0, // left
        7, 6, 2, 2, 3, 7  // top
    };

    memcpy( application->m_cornellBox.m_indices, &indices, sizeof( u32 ) * 30 );

    application->m_permanentPool = CreateMemoryPool( MEGABYTES(10) );
    application->m_transientPool = CreateMemoryPool( MEGABYTES(10) );

    Camera* camera = &application->m_camera;
    camera->m_far  = 20.0f;
    camera->m_near = 0.1f;
    camera->m_fov  = 85.0f;
}

void Update( Application* application )
{
    ClearMemoryPool( application->m_transientPool );

    Camera* camera = &application->m_camera;

    m4 projectionMatrix = CreateProjectionMatrix( camera->m_near, camera->m_far, WINDOW_WIDTH, WINDOW_HEIGHT, camera->m_fov );
    application->m_platform->setProjectionMatrix( application->m_renderer, projectionMatrix );
}

void Render( Application* application )
{
    DrawCall* call = AllocStruct( DrawCall, application->m_transientPool );

    call->m_numIndices  = 30;
    call->m_numVertices =  8;
    call->m_vertices    = application->m_cornellBox.m_vertices;
    call->m_indices     = application->m_cornellBox.m_indices;

    application->m_platform->submitDrawCall( application->m_renderer, call );
}

void Exit( Application* application )
{

}
