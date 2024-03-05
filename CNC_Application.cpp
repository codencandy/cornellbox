#include "CNC_Application.h"
#include "CNC_Types.h"
#include <string.h>

m4 CreateProjectionMatrix( f32 n   /* near */, 
                           f32 f   /* far  */,
                           f32 w   /* screen width */,
                           f32 h   /* screen height */,
                           f32 fov /* field of view angle in degrees */)
{

   f32 rad = (M_PI / 180.0f ) * fov;
   f32 asp = w/h;
   f32 t   = tanf( rad * 0.5f );

   f32 m1 = f + n;
   f32 m2 = -(n*f);

   v4 row1 = {   n,   0.0,   0.0,   0.0 }; // -> n*x
   v4 row2 = { 0.0,     n,   0.0,   0.0 }; // -> n*y
   v4 row3 = { 0.0,   0.0,    m1,    m2 }; // -> z^2 
   v4 row4 = { 0.0,   0.0,   1.0,   0.0 }; // -> w = z

   // after this matrix is multiplied each elements gets divided by w
   // x' = (n*x) / w
   // y' = (n*y) / w
   // z' = z^2   / w  with w=z => z' = z^2 / z = z ( this just preserves the original z value)
   // w  = z
   return simd_matrix_from_rows( row1, row2, row3, row4 );
}

void Load( Application* application )
{
    // construct a box => 8 corners
    f32 left   = -10.0f;
    f32 right  =  10.0f;
    f32 bottom =   0.0f;
    f32 top    =  10.0f;
    f32 far    =  10.0f;
    f32 near   = -10.0f;

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
    /*
        P4 ---- P3
        |       | 
        |       | 
        P1 ---- P2
     */
    u32 indices[12] = { 
        //0, 1, 2, 2, 3, 0,
    
    /*
          P5 ---- P6
         /       /
        P1 ---- P2
     */
         //0, 1, 5, 5, 4, 0,

    /*
          P8 ---- P7
          |       |
          |       | 
          P5 ---- P6
     */
         4, 5, 6, 6, 7, 4,

    /*
          P8 ---- P7
         /        /
        P4 ---- P3 
     */
        //3, 2, 6, 6, 7, 3,

    /*
          P8
         / | 
        P4 |
        | P5
        |/ 
        P1 
     */
        //4, 0, 3, 3, 7, 4,

    /*
           P7
          /|
        P3 | 
           P6
        | /
        P2
     */
        //1, 5, 6, 6, 2, 1,
    };

    memcpy( application->m_cornellBox.m_indices, &indices, sizeof( u32 ) * 6 );

    application->m_permanentPool = CreateMemoryPool( MEGABYTES(10) );
    application->m_transientPool = CreateMemoryPool( MEGABYTES(10) );

    m4 projectionMatrix = CreateProjectionMatrix( 0.1f, 10.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 60.0f );
    application->m_platform->setProjectionMatrix( application->m_renderer, projectionMatrix );
}

void Update( Application* application )
{
   ClearMemoryPool( application->m_transientPool );
}

void Render( Application* application )
{
   DrawCall* call = AllocStruct( DrawCall, application->m_transientPool );

   call->m_numIndices  = 6;
   call->m_numVertices = 8;
   call->m_vertices    = application->m_cornellBox.m_vertices;
   call->m_indices     = application->m_cornellBox.m_indices;

   application->m_platform->submitDrawCall( application->m_renderer, call );
}

void Exit( Application* application )
{

}
