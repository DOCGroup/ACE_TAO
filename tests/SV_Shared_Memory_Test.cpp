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

#include "ace/Malloc.h"
#include "test_config.h"

#if defined (ACE_HAS_SYSV_IPC)

// Shared memory allocator (note that this chews up the
// ACE_DEFAULT_SEM_KEY).
static ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple> allocator;

const int SEM_KEY_1 = ACE_DEFAULT_SEM_KEY + 1;
const int SEM_KEY_2 = ACE_DEFAULT_SEM_KEY + 2;
const int SHMSZ = 27;

static int
parent (char *shm)
{
  ACE_SV_Semaphore_Complex mutex;

  ACE_ASSERT (mutex.open (SEM_KEY_1,
			   ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);

  ACE_SV_Semaphore_Complex synch;
  ACE_ASSERT (synch.open (SEM_KEY_2,
			   ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);

  char *s = shm;

  for (char c = 'a'; c <= 'z'; c++)
    *s++ = c;

  *s = '\0';

  if (mutex.release () == -1)
    ACE_ERROR ((LM_ERROR, "(%P) %p", "parent mutex.release"));
  else if (synch.acquire () == -1)
    ACE_ERROR ((LM_ERROR, "(%P) %p", "parent synch.acquire"));

  if (allocator.remove () == -1)
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
  ACE_ASSERT (mutex.open (SEM_KEY_1,
			   ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);

  ACE_SV_Semaphore_Complex synch;

  ACE_ASSERT (synch.open (SEM_KEY_2,
			   ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);

  while (mutex.tryacquire () == -1)
    if (errno == EAGAIN)
      ACE_DEBUG ((LM_DEBUG, "(%P) spinning in child!\n"));
    else
      ACE_ERROR_RETURN ((LM_ERROR, "(%P) child mutex.tryacquire"), 1);

  char t = 'a';
  for (char *s = (char *) shm; *s != '\0'; s++)
    {
      ACE_ASSERT (t == s[0]);
      t++;
    }

  if (synch.release () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P) child synch.release"), 1);
  return 0;
}
#endif /* ACE_HAS_SYSV_IPC */

int
main (int, char *argv[])
{
  ACE_START_TEST ("SV_Shared_Memory_Test");

#if defined (ACE_HAS_SYSV_IPC)
  char *shm = (char *) allocator.malloc (27);

  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "(%P) fork failed\n"), -1);
      /* NOTREACHED */
    case 0:
      // Child.
      ACE_LOG_MSG->sync ("SV_Shared_Memory_Test.cpp");
      return child (shm);
    default:
      return parent (shm);
    }
#else
  ACE_ERROR ((LM_ERROR, 
	      "SYSV IPC is not supported on this platform\n"));
#endif /* ACE_HAS_SYSV_IPC */
  ACE_END_TEST;
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
