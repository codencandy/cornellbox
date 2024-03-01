#include "CNC_Memory.h"
#include <sys/malloc.h>
#include <assert.h>

struct MemoryPool* CreateMemoryPool( u32 sizeInBytes )
{
    u32 realSize     = sizeInBytes + sizeof( struct MemoryPool );
    MemoryPool* pool = (MemoryPool*)malloc( realSize );

    pool->m_size   = sizeInBytes;
    pool->m_used   = 0;
    pool->m_memory = pool + sizeof( MemoryPool );

    return pool;
}

void* AllocateStruct( u32 size, struct MemoryPool* pool )
{   
    assert( pool->m_size - pool->m_used >= size );

    void* memory  = (u8*)pool->m_memory + pool->m_used;
    pool->m_used += size;

    return memory;
}