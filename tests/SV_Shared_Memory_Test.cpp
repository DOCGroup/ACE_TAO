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
//     This is a simple test of ACE_SV_Shared_Memory and ACE_Malloc
//     using the ACE_Shared_Memory_Pool.
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
//
// ============================================================================

#include "test_config.h"
#include "ace/Malloc.h"

ACE_RCSID(tests, SV_Shared_Memory_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_SYSV_IPC) && !defined(ACE_LACKS_SYSV_SHMEM)

// Shared memory allocator (note that this chews up the
// ACE_DEFAULT_SEM_KEY).  Hide the allocator inside this function so
// that it doesn't get constructed until after the ACE_Object_Manager
// gets constructed, even with ACE_HAS_NONSTATIC_OBJECT_MANAGER.
static
ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple> &
myallocator ()
{
  static ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple> myallocator;
  return myallocator;
}

const int SEM_KEY_1 = ACE_DEFAULT_SEM_KEY + 1;
const int SEM_KEY_2 = ACE_DEFAULT_SEM_KEY + 2;

const int SHMSZ = 27;
const char SHMDATA[SHMSZ] = "abcdefghijklmnopqrstuvwxyz";

static int
parent (char *shm)
{
  ACE_SV_Semaphore_Complex mutex;

  // This semaphore is initially created with a count of 0, i.e., it
  // is "locked."
  ACE_ASSERT (mutex.open (SEM_KEY_1,
                           ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);

  ACE_SV_Semaphore_Complex synch;
  // This semaphore is initially created with a count of 0, i.e., it
  // is "locked."
  ACE_ASSERT (synch.open (SEM_KEY_2,
                           ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);

  // This for loop executes in a critical section proteced by <mutex>.
  for (int i = 0; i < SHMSZ; i++)
    shm[i] = SHMDATA[i];

  if (mutex.release () == -1)
    ACE_ERROR ((LM_ERROR, "(%P) %p", "parent mutex.release"));
  else if (synch.acquire () == -1)
    ACE_ERROR ((LM_ERROR, "(%P) %p", "parent synch.acquire"));

  if (myallocator ().remove () == -1)
    ACE_ERROR ((LM_ERROR, "(%P) %p\n", "parent allocator.remove"));
  if (mutex.remove () == -1)
    ACE_ERROR ((LM_ERROR, "(%P) %p\n", "parent mutex.remove"));
  if (synch.remove () == -1)
    ACE_ERROR ((LM_ERROR, "(%P) %p\n", "parent synch.remove"));
  return 0;
}

static int
child (char *shm)
{
  ACE_SV_Semaphore_Complex mutex;
  // This semaphore is initially created with a count of 0, i.e., it
  // is "locked."
  ACE_ASSERT (mutex.open (SEM_KEY_1,
                           ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);

  ACE_SV_Semaphore_Complex synch;
  // This semaphore is initially created with a count of 0, i.e., it
  // is "locked."
  ACE_ASSERT (synch.open (SEM_KEY_2,
                           ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);

  // Perform "busy waiting" here until we acquire the semaphore.  This
  // isn't really a good design -- it's just to illustrate that you
  // can do non-blocking acquire() calls with the ACE System V
  // semaphore wrappers.
  while (mutex.tryacquire () == -1)
    if (errno == EAGAIN)
      ACE_DEBUG ((LM_DEBUG, "(%P) spinning in child!\n"));
    else
      ACE_ERROR_RETURN ((LM_ERROR, "(%P) child mutex.tryacquire"), 1);

  for (int i = 0; i < SHMSZ; i++)
    ACE_ASSERT (SHMDATA[i] == shm[i]);

  if (synch.release () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P) child synch.release"), 1);
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple>;
template class ACE_Guard<ACE_SV_Semaphore_Simple>;
template class ACE_Write_Guard<ACE_SV_Semaphore_Simple>;
template class ACE_Read_Guard<ACE_SV_Semaphore_Simple>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Guard<ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Write_Guard<ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Read_Guard<ACE_SV_Semaphore_Simple>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_HAS_SYSV_IPC */
int
main (int, char *[])
{
  ACE_START_TEST ("SV_Shared_Memory_Test");

#if defined (ACE_HAS_SYSV_IPC) && !defined (ACE_LACKS_FORK) && !defined(ACE_LACKS_SYSV_SHMEM)
  char *shm = (char *) myallocator ().malloc (27);

  switch (ACE_OS::fork ("SV_Shared_Memory_Test.cpp"))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "(%P) fork failed\n"), -1);
      /* NOTREACHED */
    case 0:
      child (shm);
      break;
    default:
      parent (shm);
      break;
    }
#else
  ACE_ERROR ((LM_INFO,
              "SYSV IPC, SYSV SHMEM, or fork are not supported on this platform\n"));
#endif /* ACE_HAS_SYSV_IPC */
  ACE_END_TEST;
  return 0;
}
