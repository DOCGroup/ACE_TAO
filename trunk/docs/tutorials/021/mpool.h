
// $Id$

#ifndef MPOOL_H
#define MPOOL_H

// Everything else we need is in this one header
#include "ace/Malloc.h"

#if !defined (ACE_LACKS_SYSV_SHMEM)

/*
  With this we will abstract away some of the details of the memory
  pool.  Note that we don't treat this as a singleton because an
  application may need more than one pool.  Each would have a
  different name and be used for different purposes.
 */

class Allocator
{
public:
  // The pool name will be used to create a unique semaphore to
  // keep this pool separate from others.
  Allocator (const char * _name = "MemoryPool");
  ~Allocator (void);

  typedef ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_SV_Semaphore_Simple> pool_t;

  // Provide an accessor to the pool.  This will also allocate the
  // pool when first invoked.
  pool_t &pool (void);

protected:

  // The name we gave to the pool
  char *name_;
    
  pool_t *pool_;
};

/*
  The client and server need to agree on a certain set of values.  By
  placing them in the Constants class we can eliminate a bit of confusion.
 */
class Constants
{
public:
  // The semaphore keys are needed for the two semaphores that
  // synch access to the shared memory area.
  static const int SEM_KEY_1;
  static const int SEM_KEY_2;

  // How big the pool will be and what we'll put into it.  A real
  // app wouldn't need SHMDATA of course.
  static const int SHMSZ;
  static const char *SHMDATA;

  // The name assigned to the memory pool by the server is needed
  // by the client.  Without it, the pool cannot be found.
  // Likewise, the name the server will bind() to the region of the 
  // pool must be available to the client.
  static const char *PoolName;
  static const char *RegionName;
};

#endif /* ACE_LACKS_SYSV_SHMEM */
#endif /* MPOOL_H */
