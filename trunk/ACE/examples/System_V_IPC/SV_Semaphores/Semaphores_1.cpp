// $Id$

#include "ace/SV_Shared_Memory.h"
#include "ace/SV_Semaphore_Simple.h"
#include "ace/SV_Semaphore_Complex.h"
#include "ace/Shared_Memory_Pool.h"
#include "ace/Malloc_T.h"
#include "ace/OS_NS_unistd.h"

#if defined (ACE_HAS_SYSV_IPC) && !defined(ACE_LACKS_SYSV_SHMEM)

// Shared memory allocator (note that this chews up the
// ACE_DEFAULT_SEM_KEY).
static ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple> alloc;

const int SEM_KEY = ACE_DEFAULT_SEM_KEY + 1;

static int
parent (char *shm)
{
  char *s = shm;

  ACE_SV_Semaphore_Complex sem (SEM_KEY, ACE_SV_Semaphore_Complex::ACE_CREATE, 0, 2);

  for (char c = 'a'; c <= 'z'; c++)
    *s++ = c;

  *s = '\0';

  if (sem.release (0) == -1)
    ACE_ERROR ((LM_ERROR, "%p", "parent sem.release(0)"));
  else if (sem.acquire (1) == -1)
    ACE_ERROR ((LM_ERROR, "%p", "parent sem.acquire(1)"));

  if (alloc.remove () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "alloc.remove"));
  if (sem.remove () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "sem.remove"));
  return 0;
}

static int
child (char *shm)
{
  ACE_SV_Semaphore_Complex sem (SEM_KEY, ACE_SV_Semaphore_Complex::ACE_CREATE, 0, 2);

  while (sem.tryacquire (0) == -1)
    if (errno == EAGAIN)
      ACE_DEBUG ((LM_DEBUG, "spinning in client!\n"));
    else
      ACE_ERROR_RETURN ((LM_ERROR, "client mutex.tryacquire(0)"), 1);

  for (char *s = (char *) shm; *s != '\0'; s++)
    ACE_DEBUG ((LM_DEBUG, "%c", *s));

  ACE_DEBUG ((LM_DEBUG, "\n"));

  if (sem.release (1) < 0)
    ACE_ERROR ((LM_ERROR, "client sem.release(1)"));
  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  char *shm = (char *) alloc.malloc (27);

  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "fork failed\n"), -1);
      /* NOTREACHED */
    case 0:
      // Child.
      return child (shm);
    default:
      return parent (shm);
    }
}
#else
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR,
              "SYSV IPC, or SYSV SHMEM is not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_SYSV_IPC */

