
// $Id$

#ifndef MPOOL_H
#define MPOOL_H

#include "ace/Malloc.h"
#include "ace/Singleton.h"

class Allocator
{
public:
    Allocator( const char * _name = "MemoryPool" );
    ~Allocator(void);

    typedef ACE_Malloc<ACE_MMAP_Memory_Pool, ACE_SV_Semaphore_Simple> pool_t;

    pool_t & pool(void);

protected:

    char * name_;
    pool_t * pool_;
};

class Constants
{
public:
    static const int SEM_KEY_1;
    static const int SEM_KEY_2;
    
    static const int SHMSZ;
    static const char * SHMDATA;

    static const char * PoolName;
    static const char * RegionName;
};

#endif
