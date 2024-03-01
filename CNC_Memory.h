#ifndef CNC_MEMORY_H
#define CNC_MEMORY_H

#include "CNC_Types.h"

#define KILOBYTE 1024
#define MEGABYTES(x) (x*KILOBYTE)

struct MemoryPool
{
    u32   m_size;
    u32   m_used;
    void* m_memory;
};

struct MemoryPool* CreateMemoryPool( u32 sizeInBytes );
void*              AllocateStruct( u32 sizeInBytes, struct MemoryPool* pool );

#endif//CNC_MEMORY_H
