// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    SV_Shared_Memory_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of <ACE_SV_Shared_Memory> and
//     <ACE_Malloc> using the <ACE_Shared_Memory_Pool>.  The test
//     forks two processes and then executes client and server
//     allowing them to exchange data using shared memory. No user
//     input is required as far as command line arguments are
//     concerned.
//
// = AUTHOR
//    Prashant Jain <pjain@cs.wustl.edu> 
//    and Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Malloc.h"

ACE_RCSID(tests, SV_Shared_Memory_Test, "$Id$")

#if defined (ACE_HAS_SYSV_IPC) && !defined(ACE_LACKS_SYSV_SHMEM)

// The shared memory allocator, which uses up the ACE_DEFAULT_SEM_KEY.
// We hide the allocator inside this function so that it doesn't get
// constructed until after the ACE_Object_Manager gets constructed,
// even with ACE_HAS_NONSTATIC_OBJECT_MANAGER.

static
ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple> &
myallocator (void)
{
  static ACE_Malloc<ACE_SHARED_MEMORY_POOL,
                    ACE_SV_Semaphore_Simple> myallocator;
  return myallocator;
}

// Create some more keys that are different from the
// ACE_DEFAULT_SEM_KEY used by the allocator.
static const int SEM_KEY_1 = ACE_DEFAULT_SEM_KEY + 1;
static const int SEM_KEY_2 = ACE_DEFAULT_SEM_KEY + 2;

static const int SHMSZ = 27;
static const char SHMDATA[SHMSZ] = "abcdefghijklmnopqrstuvwxyz";

static ACE_SV_Semaphore_Complex *parent_mutex = 0;
static ACE_SV_Semaphore_Complex *parent_synch = 0;

static int
parent (char *shm)
{
  // This for loop executes in a critical section proteced by
  // <parent_mutex>.
  for (int i = 0; i < SHMSZ; i++)
    shm[i] = SHMDATA[i];

  int result;
  result = parent_mutex->release ();
  ACE_ASSERT (result != -1);
  
  result = parent_synch->acquire ();
  ACE_ASSERT (result != -1);  

  result = myallocator ().remove ();
  ACE_ASSERT (result != -1);  

  result = parent_mutex->remove ();
  ACE_ASSERT (result != -1);    

  result = parent_synch->remove ();
  ACE_ASSERT (result != -1);

  return 0;
}

static int
child (char *shm)
{
  int result;

  ACE_SV_Semaphore_Complex mutex;

  // This semaphore is initially created with a count of 0, i.e., it
  // is "locked."
  result = mutex.open (SEM_KEY_1,
                       ACE_SV_Semaphore_Complex::ACE_CREATE,
                       0);
  ACE_ASSERT (result != -1);

  ACE_SV_Semaphore_Complex synch;
  // This semaphore is initially created with a count of 0, i.e., it
  // is "locked."
  result = synch.open (SEM_KEY_2,
                       ACE_SV_Semaphore_Complex::ACE_CREATE,
                       0);
  ACE_ASSERT (result != -1);

  // Perform "busy waiting" here until we acquire the semaphore.  This
  // isn't really a good design -- it's just to illustrate that you
  // can do non-blocking acquire() calls with the ACE System V
  // semaphore wrappers.
  while ((result = mutex.tryacquire ()) == -1)
    if (errno == EAGAIN)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P) spinning in child!\n")));
    else
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P) child mutex.tryacquire")));
        ACE_ASSERT (result != -1);
      }

  for (int i = 0; i < SHMSZ; i++)
    ACE_ASSERT (SHMDATA[i] == shm[i]);

  result = synch.release ();
  ACE_ASSERT (result != -1);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple>;
template class ACE_Malloc_T<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple, ACE_Control_Block>;
template class ACE_Guard<ACE_SV_Semaphore_Simple>;
template class ACE_Write_Guard<ACE_SV_Semaphore_Simple>;
template class ACE_Read_Guard<ACE_SV_Semaphore_Simple>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Malloc_T<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple, ACE_Control_Block>
#pragma instantiate ACE_Guard<ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Write_Guard<ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Read_Guard<ACE_SV_Semaphore_Simple>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_HAS_SYSV_IPC */
int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SV_Shared_Memory_Test"));

#if defined (ACE_HAS_SYSV_IPC) && !defined (ACE_LACKS_FORK) && \
    !defined(ACE_LACKS_SYSV_SHMEM)
  char *shm = ACE_reinterpret_cast (char *,
                                    myallocator ().malloc (SHMSZ));

  // Create the mutex and synch before spawning the child process, to
  // avoid race condition between their creation in the parent and use
  // in the child.
  ACE_NEW_RETURN (parent_mutex,
                  ACE_SV_Semaphore_Complex,
                  -1);
  ACE_NEW_RETURN (parent_synch,
                  ACE_SV_Semaphore_Complex,
                  -1);

  // This semaphore is initially created with a count of 0, i.e., it
  // is "locked."
  ACE_ASSERT (parent_mutex->open (SEM_KEY_1,
                                  ACE_SV_Semaphore_Complex::ACE_CREATE,
                                  0) != -1);
  // This semaphore is initially created with a count of 0, i.e., it
  // is "locked."
  ACE_ASSERT (parent_synch->open (SEM_KEY_2,
                                  ACE_SV_Semaphore_Complex::ACE_CREATE,
                                  0) != -1);
  switch (ACE_OS::fork ("SV_Shared_Memory_Test.cpp"))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P) fork failed\n")),
                        -1);
      /* NOTREACHED */
    case 0:
      child (shm);
      break;
    default:
      parent (shm);
      delete parent_mutex;
      delete parent_synch;
      break;
    }
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("SYSV IPC, SYSV SHMEM, or fork ")
              ACE_TEXT ("are not supported on this platform\n")));
#endif /* ACE_HAS_SYSV_IPC */
  ACE_END_TEST;
  return 0;
}
