#include "Options.h"
// $Id$

#include "Malloc.h"

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

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Allocator_Adapter<L_ALLOCATOR>;
template class ACE_Allocator_Adapter<M_ALLOCATOR>;
template class ACE_Allocator_Adapter<SB_ALLOCATOR>;
template class ACE_Allocator_Adapter<SP_ALLOCATOR>;
template class ACE_Allocator_Adapter<ST_ALLOCATOR>;
template class ACE_Guard<ACE_Process_Mutex>;
template class ACE_Malloc <ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX>;
template class ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex>;
template class ACE_Read_Guard<ACE_Process_Mutex>;
template class ACE_Read_Guard<ACE_SYNCH_MUTEX>;
template class ACE_Write_Guard<ACE_Process_Mutex>;
template class ACE_Write_Guard<ACE_SYNCH_MUTEX>;

#if defined (ACE_LACKS_SYSV_SHMEM)
template class ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_SYNCH_MUTEX>;
#else
template class ACE_Malloc <ACE_SHARED_MEMORY_POOL, ACE_Process_Mutex>;
template class ACE_Malloc <ACE_SHARED_MEMORY_POOL, ACE_SYNCH_MUTEX>;
#endif /* ACE_LACKS_SYSV_SHMEM */

#if defined (ACE_LACKS_SBRK)
template class ACE_Malloc <ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX>;
#else
template class ACE_Malloc <ACE_SBRK_MEMORY_POOL, ACE_SYNCH_MUTEX>;
#endif /* ACE_LACKS_SBRK */
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
