#ifndef CNC_PLATFORM_H
#define CNC_PLATFORM_H

#include "CNC_Types.h"

struct Platform
{
    void (*submitDrawCall)( void*, DrawCall* );
};

// services the platform provides to the application
void SubmitDrawCall( void* renderer, DrawCall* call );

#endif//CNC_PLATFORM_H
