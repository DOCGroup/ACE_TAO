/**
 * @file Bug_3911_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problem reported in bug 3911
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=3911
 */

#include "test_config.h"

#include <ace/Malloc_T.h>
#include <ace/Memory_Pool.h>
#include <ace/Process_Mutex.h>
#include <ace/Null_Mutex.h>
#include <ace/PI_Malloc.h>

#define MMAP_FILENAME ACE_TEXT ("shared_memory")

static void
init_test ()
{
  // Cleanup the MMAP file so we won't trip over the leftover mmap
  // file from the previous crash.
#if defined (ACE_HAS_WINCE) || defined (ACE_OPENVMS)
  // WinCE cannot do fixed base, ever.
  ACE_MMAP_Memory_Pool_Options options
    (0,
     ACE_MMAP_Memory_Pool_Options::NEVER_FIXED);
#else
  ACE_MMAP_Memory_Pool_Options options (ACE_DEFAULT_BASE_ADDR);
#endif /* ACE_HAS_WINCE */
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_MMAP_Memory_Pool mmap (MMAP_FILENAME, &options);
  //FUZZ: enable check_for_lack_ACE_OS

  size_t rbyte = 0;
  int ft = 0;
  mmap.init_acquire (1024, rbyte, ft);
  mmap.release ();
}

class ShmemMan
{
private:
  // Used to implement the singleton pattern:
  static ShmemMan* c_instance;

  // The memory pool managing all the shared memory:
  ACE_Malloc_T<ACE_MMAP_MEMORY_POOL,
        ACE_Null_Mutex,
        ACE_PI_Control_Block>*    c_memory_pool;

protected:
  ShmemMan(bool no_crash)
  {
    if (no_crash)
    {
      ACE_MMAP_Memory_Pool_Options options (ACE_DEFAULT_BASE_ADDR);
      c_memory_pool = new ACE_Malloc_T<ACE_MMAP_MEMORY_POOL,
                      ACE_Null_Mutex,
                      ACE_PI_Control_Block>(MMAP_FILENAME
                                            , ACE_TEXT(""), &options
                                            );
    }
    else
    {
      c_memory_pool = new ACE_Malloc_T<ACE_MMAP_MEMORY_POOL,
                      ACE_Null_Mutex,
                      ACE_PI_Control_Block>(MMAP_FILENAME);
    }
  };

public:
  ~ShmemMan()
  {
    c_memory_pool->release();
    delete c_memory_pool;
  };

  static ShmemMan* getInstance(bool no_crash)
  {
    if (c_instance == 0)
    {
      c_instance = new ShmemMan(no_crash);
    }
    return c_instance;
  };

  int clean()
  {
    c_memory_pool->release();
    c_memory_pool->remove();

    return 0;
  };

  void* getMemoryBlock(char* block_name, unsigned int block_size)
  {
    void* shared;

    ACE_DEBUG((LM_INFO, ACE_TEXT("errno = %d. Looking for a Shared Memory block named %C\n"),
                        ACE_OS::last_error(),
                        block_name));

    if (c_memory_pool->find(block_name, shared) == 0)
    {
      // An existing block was found, so take that:
      ACE_DEBUG((LM_INFO, ACE_TEXT("Shared Memory block %C was found."),
                          block_name));
    }
    else
    {
      // Allocate the memory and bind it to a name:
      ACE_DEBUG((LM_INFO, ACE_TEXT("Shared Memory block %C was not found. errno = %d. Trying to allocate new block\n"),
                          block_name,
                          ACE_OS::last_error()));
      shared = c_memory_pool->malloc(block_size);
      if (shared < 0)
      {
        ACE_DEBUG((LM_INFO, ACE_TEXT("New Shared Memory block could not be allocated. errno = %d.\n"),
                            ACE_OS::last_error()));
        return (void*)(-1);
      }
      ACE_DEBUG((LM_INFO, ACE_TEXT("New Shared Memory block was allocated, trying to bind it to the name %C\n"),
                          block_name));
      if (c_memory_pool->bind(block_name, shared) < 0)
      {
        ACE_DEBUG((LM_INFO, ACE_TEXT("New Shared Memory block could not be bound to the name %C. errno = %d.\n"),
                            block_name,
                            ACE_OS::last_error()));

        return (void*)(-1);
      }
    }

    return shared;
  };

};

ShmemMan* ShmemMan::c_instance = 0;

// Main function.

int
run_main (int argc, ACE_TCHAR * argv[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3911_Regression_Test"));

  init_test ();

  bool no_crash = (argc>1 && argv[1][0]=='1');
  ShmemMan* smm = ShmemMan::getInstance (no_crash);

  void* buf = smm->getMemoryBlock ("block_1", 10 * 4096);

  ACE_DEBUG((LM_INFO, ACE_TEXT("allocated shmem block at %@\n"), buf));

  smm->clean ();

  ACE_END_TEST;
  return 0;
}

