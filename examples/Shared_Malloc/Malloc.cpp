#include "Options.h"
// $Id$

#include "Malloc.h"

// Determine the type of dynamic memory manager.
#if defined (ACE_HAS_THREADS)
typedef ACE_Thread_Mutex THREAD_MUTEX;
#else
typedef ACE_Null_Mutex THREAD_MUTEX;
#endif /* ACE_HAS_THREADS */

// ACE_Process_Mutex will work for non-MT platforms.
typedef ACE_Process_Mutex PROCESS_MUTEX;

class MMAP_POOL : public ACE_MMAP_Memory_Pool
  // = TITLE
  //   This class is an "adapter" that passes certain arguments to the
  //   constructor of ACE_MMAP_Memory_Pool.
{
public:
  MMAP_POOL (const char *);

  static const char BACKING_STORE[];
};

const char MMAP_POOL::BACKING_STORE[] = "/tmp/test_malloc";

MMAP_POOL::MMAP_POOL (const char *) 
  : ACE_MMAP_Memory_Pool (BACKING_STORE, // Name of the backing store.
			  1, // Use fixed addr.
			  1) // Write each page of backing store.
{
}

// Strategic typedefs for memory allocation.

typedef ACE_Malloc <ACE_Local_Memory_Pool, THREAD_MUTEX> L_ALLOCATOR;

#if !defined (ACE_WIN32)
typedef ACE_Malloc <MMAP_POOL, PROCESS_MUTEX> M_ALLOCATOR;
typedef ACE_Malloc <ACE_Shared_Memory_Pool, PROCESS_MUTEX> SP_ALLOCATOR;
typedef ACE_Malloc <ACE_Shared_Memory_Pool, THREAD_MUTEX> ST_ALLOCATOR;
typedef ACE_Malloc <ACE_Sbrk_Memory_Pool, THREAD_MUTEX> SB_ALLOCATOR;
#else
typedef ACE_Malloc <MMAP_POOL, THREAD_MUTEX> M_ALLOCATOR;
typedef ACE_Malloc <MMAP_POOL, THREAD_MUTEX> SP_ALLOCATOR;
typedef ACE_Malloc <MMAP_POOL, THREAD_MUTEX> ST_ALLOCATOR;
typedef ACE_Malloc <ACE_Local_Memory_Pool, THREAD_MUTEX> SB_ALLOCATOR;
#endif /* ACE_WIN32 */

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
