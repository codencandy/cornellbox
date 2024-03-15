#ifndef CNC_PLATFORM_H
#define CNC_PLATFORM_H

#include "CNC_Types.h"

struct Platform
{
    void (*submitDrawCall)( void*, DrawCall* );
    void (*setCameraData)( void*, m4, m4, m4, struct Quarternion, struct Quarternion );
    void (*reloadShaders)( void* );
};

// services the platform provides to the application
void SubmitDrawCall( void* renderer, DrawCall* call );
void SetCameraData( void* renderer, m4 projectionMatrix, m4 viewTransform, m4 scaleTransform, struct Quarternion rotation, struct Quarternion inverse );
void ReloadShaders( void* renderer );
#endif//CNC_PLATFORM_H
