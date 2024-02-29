#include "CNC_Application.h"

void Load( Application* application )
{
    // construct a box => 8 corners
    f32 left   = -10.0f;
    f32 right  =  10.0f;
    f32 bottom =   0.0f;
    f32 top    =  10.0f;
    f32 far    =  10.0f;
    f32 near   =   0.0f;

    /*
          P8 ---- P7
         / |      /|
        P4 ---- P3 | 
        | P5 ---|- P6
        |/      | /
        P1 ---- P2
     */

    v3 P1 = { left,  bottom, near };
    v3 P2 = { right, bottom, near };
    v3 P3 = { right, top,    near };
    v3 P4 = { left,  top,    near };

    v3 P5 = { left,  bottom, far };
    v3 P6 = { right, bottom, far };
    v3 P7 = { right, top,    far };
    v3 P8 = { left,  top,    far };

    application->m_cornellBox.m_vertices[0].m_position = P1;
    application->m_cornellBox.m_vertices[1].m_position = P2;
    application->m_cornellBox.m_vertices[2].m_position = P3;
    application->m_cornellBox.m_vertices[3].m_position = P4;
    application->m_cornellBox.m_vertices[4].m_position = P5;
    application->m_cornellBox.m_vertices[5].m_position = P6;
    application->m_cornellBox.m_vertices[6].m_position = P7;
    application->m_cornellBox.m_vertices[7].m_position = P8;
}

void Update( Application* application )
{

}

void Render( Application* application )
{

}

void Exit( Application* application )
{

}
