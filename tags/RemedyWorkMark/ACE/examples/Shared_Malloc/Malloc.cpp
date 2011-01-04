// $Id$

#include "Options.h"
#include "Malloc.h"
#include "ace/MMAP_Memory_Pool.h"
#include "ace/Local_Memory_Pool.h"

#ifndef ACE_LACKS_SYSV_SHMEM
# include "ace/Shared_Memory_Pool.h"
#endif  /* !ACE_LACKS_SYSV_SHMEM */

#ifndef ACE_LACKS_SBRK
# include "ace/Sbrk_Memory_Pool.h"
#endif  /* !ACE_LACKS_SBRK */

#include "ace/Process_Mutex.h"
#include "ace/Malloc_T.h"



// Strategic typedefs for memory allocation.

typedef ACE_Malloc <ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX> L_ALLOCATOR;
typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> M_ALLOCATOR;

#if defined (ACE_LACKS_SYSV_SHMEM)
typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_SYNCH_MUTEX> SP_ALLOCATOR;
typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_SYNCH_MUTEX> ST_ALLOCATOR;
#else
typedef ACE_Malloc <ACE_SHARED_MEMORY_POOL, ACE_Process_Mutex> SP_ALLOCATOR;
typedef ACE_Malloc <ACE_SHARED_MEMORY_POOL, ACE_SYNCH_MUTEX> ST_ALLOCATOR;
#endif /* ACE_LACKS_SYSV_SHMEM */

#if defined (ACE_LACKS_SBRK)
typedef ACE_Malloc <ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX> SB_ALLOCATOR;
#else
typedef ACE_Malloc <ACE_SBRK_MEMORY_POOL, ACE_SYNCH_MUTEX> SB_ALLOCATOR;
#endif /* ACE_LACKS_SBRK */

// Singleton
ACE_Allocator *Malloc::instance_ = 0;

// This is a factory that decides what type of allocator to create.

ACE_Allocator *
Malloc::instance (void)
{
  if (Malloc::instance_ == 0)
    {
      if (Options::instance ()->child ())
        Malloc::instance_ = new ACE_Allocator_Adapter<M_ALLOCATOR>;
      else if (Options::instance ()->spawn_threads ())
        {
          if (Options::instance ()->use_sbrk ())
            Malloc::instance_ = new ACE_Allocator_Adapter<SB_ALLOCATOR>;
          else if (Options::instance ()->use_shmem ())
            Malloc::instance_ = new ACE_Allocator_Adapter<ST_ALLOCATOR>;
          else
            Malloc::instance_ = new ACE_Allocator_Adapter<L_ALLOCATOR>;
        }
      else if (Options::instance ()->use_mmap ())
        Malloc::instance_ = new ACE_Allocator_Adapter<M_ALLOCATOR>;
      else // Use Shared_Memory_Pool.
        Malloc::instance_ = new ACE_Allocator_Adapter<SP_ALLOCATOR>;
    }

  return Malloc::instance_;
}

