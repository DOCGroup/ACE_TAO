#include "Options.h"
// $Id$

#include "Malloc.h"

// Determine the type of dynamic memory manager.
#if defined (ACE_HAS_THREADS)
typedef ACE_Thread_Mutex THREAD_MUTEX;
#else
typedef ACE_Null_Mutex THREAD_MUTEX;
#endif /* ACE_HAS_THREADS */

// Strategic typedefs for memory allocation.

typedef ACE_Malloc <ACE_LOCAL_MEMORY_POOL, THREAD_MUTEX> L_ALLOCATOR;
typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> M_ALLOCATOR;

#if defined (ACE_LACKS_SYSV_SHMEM)
typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, THREAD_MUTEX> SP_ALLOCATOR;
typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, THREAD_MUTEX> ST_ALLOCATOR;
#else
typedef ACE_Malloc <ACE_SHARED_MEMORY_POOL, ACE_Process_Mutex> SP_ALLOCATOR;
typedef ACE_Malloc <ACE_SHARED_MEMORY_POOL, THREAD_MUTEX> ST_ALLOCATOR;
#endif /* ACE_LACKS_SYSV_SHMEM */

#if defined (ACE_LACKS_SBRK)
typedef ACE_Malloc <ACE_LOCAL_MEMORY_POOL, THREAD_MUTEX> SB_ALLOCATOR;
#else
typedef ACE_Malloc <ACE_SBRK_MEMORY_POOL, THREAD_MUTEX> SB_ALLOCATOR;
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
template class ACE_Malloc <ACE_LOCAL_MEMORY_POOL, THREAD_MUTEX>;
template class ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex>;
template class ACE_Read_Guard<ACE_Process_Mutex>;
template class ACE_Read_Guard<THREAD_MUTEX>;
template class ACE_Write_Guard<ACE_Process_Mutex>;
template class ACE_Write_Guard<THREAD_MUTEX>;

#if defined (ACE_LACKS_SYSV_SHMEM)
template class ACE_Malloc <ACE_MMAP_MEMORY_POOL, THREAD_MUTEX>;
#else
template class ACE_Malloc <ACE_SHARED_MEMORY_POOL, ACE_Process_Mutex>;
template class ACE_Malloc <ACE_SHARED_MEMORY_POOL, THREAD_MUTEX>;
#endif /* ACE_LACKS_SYSV_SHMEM */

#if defined (ACE_LACKS_SBRK)
template class ACE_Malloc <ACE_LOCAL_MEMORY_POOL, THREAD_MUTEX>;
#else
template class ACE_Malloc <ACE_SBRK_MEMORY_POOL, THREAD_MUTEX>;
#endif /* ACE_LACKS_SBRK */
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
