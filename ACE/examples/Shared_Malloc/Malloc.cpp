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
#include <memory>


// Strategic typedefs for memory allocation.

using L_ALLOCATOR = ACE_Malloc <ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX>;
using M_ALLOCATOR = ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex>;

#if defined (ACE_LACKS_SYSV_SHMEM)
using SP_ALLOCATOR = ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_SYNCH_MUTEX>;
using ST_ALLOCATOR = ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_SYNCH_MUTEX>;
#else
using SP_ALLOCATOR = ACE_Malloc <ACE_SHARED_MEMORY_POOL, ACE_Process_Mutex>;
using ST_ALLOCATOR = ACE_Malloc <ACE_SHARED_MEMORY_POOL, ACE_SYNCH_MUTEX>;
#endif /* ACE_LACKS_SYSV_SHMEM */

#if defined (ACE_LACKS_SBRK)
using SB_ALLOCATOR = ACE_Malloc <ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX>;
#else
using SB_ALLOCATOR = ACE_Malloc <ACE_SBRK_MEMORY_POOL, ACE_SYNCH_MUTEX>;
#endif /* ACE_LACKS_SBRK */

// This is a factory that decides what type of allocator to create.

ACE_Allocator *
Malloc::instance ()
{
  static std::unique_ptr<ACE_Allocator> const instance = [] ()
    -> std::unique_ptr<ACE_Allocator>
    {
      Options const *const options = Options::instance ();

      if (options->child ())
        return std::make_unique<ACE_Allocator_Adapter<M_ALLOCATOR>> ();
      else if (options->spawn_threads ())
        {
          if (options->use_sbrk ())
            return std::make_unique<ACE_Allocator_Adapter<SB_ALLOCATOR>> ();
          else if (options->use_shmem ())
            return std::make_unique<ACE_Allocator_Adapter<ST_ALLOCATOR>> ();
          else
            return std::make_unique<ACE_Allocator_Adapter<L_ALLOCATOR>> ();
        }
      else if (options->use_mmap ())
        return std::make_unique<ACE_Allocator_Adapter<M_ALLOCATOR>> ();
      else // Use Shared_Memory_Pool.
        return std::make_unique<ACE_Allocator_Adapter<SP_ALLOCATOR>> ();
    } ();

  return instance.get ();
}
