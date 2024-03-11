#ifndef CNC_PLATFORM_H
#define CNC_PLATFORM_H

#include "CNC_Types.h"

struct Platform
{
    void (*submitDrawCall)( void*, DrawCall* );
    void (*setProjectionMatrix)( void*, m4 );
    void (*reloadShaders)( void* );
};

// services the platform provides to the application
void SubmitDrawCall( void* renderer, DrawCall* call );
void SetProjectionMatrix( void* renderer, m4 projectionMatrix );
void ReloadShaders( void* renderer );
#endif//CNC_PLATFORM_H
