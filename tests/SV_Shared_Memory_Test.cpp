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
  static ACE_Malloc<ACE_SHARED_MEMORY_POOL,
                    ACE_SV_Semaphore_Simple> myallocator;
  return myallocator;
}

static const int SEM_KEY_1 = ACE_DEFAULT_SEM_KEY + 1;
static const int SEM_KEY_2 = ACE_DEFAULT_SEM_KEY + 2;

static const int SHMSZ = 27;
static const char SHMDATA[SHMSZ] = "abcdefghijklmnopqrstuvwxyz";

static ACE_SV_Semaphore_Complex *parent_mutex = 0;
static ACE_SV_Semaphore_Complex *parent_synch = 0;

static int
parent (char *shm)
{
  // This for loop executes in a critical section proteced by <parent_mutex>.
  for (int i = 0; i < SHMSZ; i++)
    shm[i] = SHMDATA[i];

  if (parent_mutex->release () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P) %p"),
                ASYS_TEXT ("parent mutex.release")));
  else if (parent_synch->acquire () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P) %p"),
                ASYS_TEXT ("parent synch.acquire")));

  if (myallocator ().remove () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P) %p\n"),
                ASYS_TEXT ("parent allocator.remove")));
  if (parent_mutex->remove () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P) %p\n"),
                ASYS_TEXT ("parent mutex.remove")));
  if (parent_synch->remove () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P) %p\n"),
                ASYS_TEXT ("parent synch.remove")));
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
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P) spinning in child!\n")));
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%P) child mutex.tryacquire")),
                        1);

  for (int i = 0; i < SHMSZ; i++)
    ACE_ASSERT (SHMDATA[i] == shm[i]);

  if (synch.release () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%P) child synch.release")),
                      1);
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
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("SV_Shared_Memory_Test"));

#if defined (ACE_HAS_SYSV_IPC) && !defined (ACE_LACKS_FORK) && \
    !defined(ACE_LACKS_SYSV_SHMEM)
  char *shm = ACE_reinterpret_cast (char *, myallocator ().malloc (SHMSZ));

  // Create the mutex and synch before spawning the child process, to
  // avoid race condition between their creation in the parent and use
  // in the child.
  ACE_NEW_RETURN (parent_mutex, ACE_SV_Semaphore_Complex, -1);
  ACE_NEW_RETURN (parent_synch, ACE_SV_Semaphore_Complex, -1);

  // This semaphore is initially created with a count of 0, i.e., it
  // is "locked."
  ACE_ASSERT (parent_mutex->open (SEM_KEY_1,
                          ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);

  // This semaphore is initially created with a count of 0, i.e., it
  // is "locked."
  ACE_ASSERT (parent_synch->open (SEM_KEY_2,
                          ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);


  switch (ACE_OS::fork ("SV_Shared_Memory_Test.cpp"))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%P) fork failed\n")),
                        -1);
      /* NOTREACHED */
    case 0:
      child (shm);
      break;
    default:
      parent (shm);
      break;
    }

  delete parent_mutex;
  delete parent_synch;
#else
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("SYSV IPC, SYSV SHMEM, or fork ")
              ASYS_TEXT ("are not supported on this platform\n")));
#endif /* ACE_HAS_SYSV_IPC */
  ACE_END_TEST;
  return 0;
}
