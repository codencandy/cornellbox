#ifndef CNC_APPLICATION_H
#define CNC_APPLICATION_H

#include "CNC_Types.h"
#include "CNC_Memory.h"
#include "CNC_Platform.h"

struct Application
{
    MemoryPool* m_permanentPool;
    MemoryPool* m_transientPool;
    
    void*       m_renderer;
    Camera      m_camera;
    Box         m_cornellBox;

    Platform*   m_platform;
};

void Load( Application* application );
void Update( Application* application );
void Render( Application* application );
void Exit( Application* application );

#endif//CNC_APPLICATION_H
