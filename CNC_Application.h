#ifndef CNC_APPLICATION_H
#define CNC_APPLICATION_H

#include "CNC_Types.h"

struct Application
{
    Camera m_camera;
};

void Load( Application* application );
void Update( Application* application );
void Render( Application* application );
void Exit( Application* application );

#endif//CNC_APPLICATION_H
