#include "CNC_Application.h"
#include "CNC_Types.h"
#include <string.h>

Quarternion toQuarterion( f32 radians, f32 x, f32 y, f32 z )
{
    Quarternion q = {};
    q.m_q0 = cosf( radians * 0.5f );
    q.m_q1 = x * sinf( radians * 0.5f );
    q.m_q2 = y * sinf( radians * 0.5f );
    q.m_q3 = z * sinf( radians * 0.5f );

    return q;
}

Quarternion toInverse( Quarternion& a )
{
    Quarternion q = {};

    q.m_q0 =  a.m_q0;
    q.m_q1 = -a.m_q1;
    q.m_q2 = -a.m_q2;
    q.m_q3 = -a.m_q3;

    return q;
}

Quarternion multiply( Quarternion& a, Quarternion& b )
{
    Quarternion result = {};

    result.m_q0 = (a.m_q0 * b.m_q0) - (a.m_q1 * b.m_q1) - (a.m_q2 * b.m_q2) - (a.m_q3 * b.m_q3);
    result.m_q1 = (a.m_q0 * b.m_q1) + (a.m_q1 * b.m_q0) - (a.m_q2 * b.m_q3) + (a.m_q3 * b.m_q2);
    result.m_q2 = (a.m_q0 * b.m_q2) + (a.m_q1 * b.m_q3) + (a.m_q2 * b.m_q0) - (a.m_q3 * b.m_q1);
    result.m_q3 = (a.m_q0 * b.m_q3) - (a.m_q1 * b.m_q2) + (a.m_q2 * b.m_q1) + (a.m_q3 * b.m_q0);

    return result;
}

Quarternion cameraRotationQuarternion( Quarternion q, f32 roll, f32 pitch, f32 yaw )
{
    f32 r = pitch  * 0.5f;
    f32 p = yaw    * 0.5f;
    f32 y = roll   * 0.5f;

    q.m_q0 = (cos(r)*cos(p)*cos(y)) + (sin(r)*sin(p)*sin(y));
    q.m_q1 = (sin(r)*cos(p)*cos(y)) - (cos(r)*sin(p)*sin(y));
    q.m_q2 = (cos(r)*sin(p)*cos(y)) + (sin(r)*cos(p)*sin(y));
    q.m_q3 = (cos(r)*cos(p)*sin(y)) - (sin(r)*sin(p)*cos(y));

    return q;
}

Quarternion leftRight( f32 radians )
{
    return toQuarterion( radians, 0.0f, 1.0f, 0.0f );
}

Quarternion operator * ( Quarternion& a, Quarternion& b )
{
    Quarternion result = {};

    result.m_q0 = (a.m_q0 * b.m_q0) - (a.m_q1 * b.m_q1) - (a.m_q2 * b.m_q2) - (a.m_q3 * b.m_q3);
    result.m_q1 = (a.m_q0 * b.m_q1) + (a.m_q1 * b.m_q0) - (a.m_q2 * b.m_q3) + (a.m_q3 * b.m_q2);
    result.m_q2 = (a.m_q0 * b.m_q2) + (a.m_q1 * b.m_q3) + (a.m_q2 * b.m_q0) - (a.m_q3 * b.m_q1);
    result.m_q3 = (a.m_q0 * b.m_q3) - (a.m_q1 * b.m_q2) + (a.m_q2 * b.m_q1) + (a.m_q3 * b.m_q0);

    return result;
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

m4 CreateTransformMatrix( v3 pos )
{
    v4 row1 = { 1.0,   0.0,   0.0,   pos[0]  }; 
    v4 row2 = { 0.0,   1.0,   0.0,   pos[1]  }; 
    v4 row3 = { 0.0,   0.0,   1.0,   pos[2]  };  
    v4 row4 = { 0.0,   0.0,   0.0,   1.0     }; 

    return simd_matrix_from_rows( row1, row2, row3, row4 );
}

m4 CreateScaleMatrix( f32 x, f32 y, f32 z )
{
    v4 row1 = {   x,   0.0,   0.0,    0.0f  };
    v4 row2 = { 0.0,     y,   0.0,    0.0f  }; 
    v4 row3 = { 0.0,   0.0,     z,    0.0f  };  
    v4 row4 = { 0.0,   0.0,   0.0,    1.0f  }; 

    return simd_matrix_from_rows( row1, row2, row3, row4 );
}

void Load( Application* application )
{
    Box* box = &application->m_cornellBox;

    box->m_width       = 20.0f;
    box->m_height      = 10.0f;
    box->m_length      = 20.0f;
    
    // construct a box => 8 corners
    f32 left   =  -1.0f;
    f32 right  =   1.0f;
    f32 bottom =  -1.0f;
    f32 top    =   1.0f;
    f32 far    =   1.0f;
    f32 near   =  -1.0f;

    /*
          P8 ---- P7
         / |      /|
        P4 ---- P3 | 
        | P5 ---|- P6
        |/      | /
        P1 ---- P2
     */

    v3 P1 = { left,  bottom, near }; // (-1, 0, 0)
    v3 P2 = { right, bottom, near }; // ( 1, 0, 0)
    v3 P3 = { right, top,    near }; // ( 1, 1, 0)
    v3 P4 = { left,  top,    near }; // (-1, 1, 0)

    v3 P5 = { left,  bottom, far };  // (-1, 0, 1)
    v3 P6 = { right, bottom, far };  // ( 1, 0, 1)
    v3 P7 = { right, top,    far };  // ( 1, 1, 1)
    v3 P8 = { left,  top,    far };  // (-1, 1, 1)

    box->m_vertices[0].m_position = P1;
    box->m_vertices[1].m_position = P2;
    box->m_vertices[2].m_position = P3;
    box->m_vertices[3].m_position = P4;
    box->m_vertices[4].m_position = P5;
    box->m_vertices[5].m_position = P6;
    box->m_vertices[6].m_position = P7;
    box->m_vertices[7].m_position = P8;

    // construct the list of indices
    u32 indices[30] = { 
        4, 5, 6, 6, 7, 4, // front
        0, 1, 5, 5, 4, 0, // bottom
        5, 1, 2, 2, 6, 5, // right
        0, 4, 7, 7, 3, 0, // left
        7, 6, 2, 2, 3, 7  // top
    };

    memcpy( box->m_indices, &indices, sizeof( u32 ) * 30 );

    application->m_permanentPool = CreateMemoryPool( MEGABYTES(10) );
    application->m_transientPool = CreateMemoryPool( MEGABYTES(10) );

    Camera* camera = &application->m_camera;
    camera->m_far               = 20.0f;
    camera->m_near              = 0.1f;
    camera->m_fov               = 85.0f;
    camera->m_screenWidth       = WINDOW_WIDTH;
    camera->m_screenHeight      = WINDOW_HEIGHT;
    camera->m_direction         = { 0.0f, 0.0f, 1.0f }; // into the screen along the z axis
    camera->m_position          = { 0.0f, 0.0f, 0.0f }; // camera at the center of the world
    camera->m_roll              = 0.0f;
    camera->m_pitch             = 0.0f;
    camera->m_yaw               = 0.0f;
}

void Update( Application* application )
{
    ClearMemoryPool( application->m_transientPool );

    Box* box           = &application->m_cornellBox;
    box->m_scaleMatrix = CreateScaleMatrix( box->m_width  * 0.5f,
                                            box->m_height * 0.5f,
                                            box->m_length * 0.5f);

    Camera*     camera = &application->m_camera;
    v3          dir    = camera->m_direction;
    Quarternion p      = toQuarterion( 0.0f, dir[0], dir[1], dir[2] );
    
    camera->m_rotationQuarternion = cameraRotationQuarternion( p, camera->m_roll, camera->m_pitch, camera->m_yaw );
    camera->m_inverseRotation     = toInverse( camera->m_rotationQuarternion );

    // update the camera direction 
    Quarternion p1 = multiply( camera->m_inverseRotation, p );
    Quarternion p2 = multiply( p1, camera->m_rotationQuarternion );
    
    camera->m_direction = { p2.m_q1, p2.m_q2, p2.m_q3 };
    
    m4 projectionMatrix = CreateProjectionMatrix( camera->m_near, camera->m_far, camera->m_screenWidth, camera->m_screenHeight, camera->m_fov );
    m4 viewTransform    = CreateTransformMatrix( camera->m_position );
    application->m_platform->setCameraData( application->m_renderer, projectionMatrix, viewTransform, box->m_scaleMatrix, camera->m_rotationQuarternion, camera->m_inverseRotation );
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
