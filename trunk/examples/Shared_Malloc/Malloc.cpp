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

ACE_RCSID(Shared_Malloc, Malloc, "$Id$")

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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Allocator_Adapter<M_ALLOCATOR>;
#if !defined (ACE_LACKS_SBRK)
template class ACE_Allocator_Adapter<SB_ALLOCATOR>;
template class ACE_Malloc <ACE_SBRK_MEMORY_POOL, ACE_SYNCH_MUTEX>;
template class ACE_Malloc_T <ACE_SBRK_MEMORY_POOL, ACE_SYNCH_MUTEX, ACE_Control_Block>;
#endif /* ! ACE_LACKS_SBRK */
#if !defined (ACE_LACKS_SYSV_SHMEM)
template class ACE_Allocator_Adapter<SP_ALLOCATOR>;
template class ACE_Allocator_Adapter<ST_ALLOCATOR>;
#endif /* ! ACE_LACKS_SYSV_SHMEM */

template class ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex>;
template class ACE_Malloc_T <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_Control_Block>;
#if !defined (ACE_HAS_THREADS) && !defined(ACE_HAS_STD_TEMPLATE_SPECIALIZATION)
template class ACE_Read_Guard<ACE_SYNCH_MUTEX>;
template class ACE_Write_Guard<ACE_SYNCH_MUTEX>;
#endif /* ! ACE_HAS_THREADS && ! ACE_HAS_STD_TEMPLATE_SPECIALIZATION */

#if defined (ACE_LACKS_SYSV_SHMEM)

#else
template class ACE_Malloc <ACE_SHARED_MEMORY_POOL, ACE_Process_Mutex>;
template class ACE_Malloc <ACE_SHARED_MEMORY_POOL, ACE_SYNCH_MUTEX>;
template class ACE_Malloc_T <ACE_SHARED_MEMORY_POOL, ACE_Process_Mutex, ACE_Control_Block>;
template class ACE_Malloc_T <ACE_SHARED_MEMORY_POOL, ACE_SYNCH_MUTEX, ACE_Control_Block>;
#endif /* ACE_LACKS_SYSV_SHMEM */

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Allocator_Adapter<L_ALLOCATOR>
#pragma instantiate ACE_Allocator_Adapter<M_ALLOCATOR>
#if !defined (ACE_LACKS_SBRK)
#pragma instantiate ACE_Allocator_Adapter<SB_ALLOCATOR>
#pragma instantiate ACE_Malloc <ACE_SBRK_MEMORY_POOL, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Malloc_T <ACE_SBRK_MEMORY_POOL, ACE_SYNCH_MUTEX, ACE_Control_Block>
#endif /* ! ACE_LACKS_SYSV_SHMEM */
#if !defined (ACE_LACKS_SBRK)
#pragma instantiate ACE_Allocator_Adapter<SP_ALLOCATOR>
#endif /* ! ACE_LACKS_SYSV_SHMEM */
#pragma instantiate ACE_Allocator_Adapter<ST_ALLOCATOR>
#pragma instantiate ACE_Malloc <ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex>
#pragma instantiate ACE_Malloc_T <ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX, ACE_Control_Block>
#pragma instantiate ACE_Malloc_T <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_Control_Block>
#pragma instantiate ACE_Read_Guard<ACE_Process_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_Process_Mutex>
#if !defined (ACE_HAS_THREADS)
#pragma instantiate ACE_Read_Guard<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Write_Guard<ACE_SYNCH_MUTEX>
#endif /* ! ACE_HAS_THREADS */

#if defined (ACE_LACKS_SYSV_SHMEM)
#pragma instantiate ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Malloc_T <ACE_MMAP_MEMORY_POOL, ACE_SYNCH_MUTEX, ACE_Control_Block>
#else
#pragma instantiate ACE_Malloc <ACE_SHARED_MEMORY_POOL, ACE_Process_Mutex>
#pragma instantiate ACE_Malloc <ACE_SHARED_MEMORY_POOL, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Malloc_T <ACE_SHARED_MEMORY_POOL, ACE_Process_Mutex, ACE_Control_Block>
#pragma instantiate ACE_Malloc_T <ACE_SHARED_MEMORY_POOL, ACE_SYNCH_MUTEX, ACE_Control_Block>
#endif /* ACE_LACKS_SYSV_SHMEM */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
