// $Id$

#include "ace/SV_Shared_Memory.h"
#include "ace/SV_Semaphore_Simple.h"
#include "ace/Malloc.h"

#if defined (ACE_HAS_SYSV_IPC)

// Shared memory allocator (note that this chews up the
// ACE_DEFAULT_SEM_KEY).
static ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple> allocator;

const int SEM_KEY = ACE_DEFAULT_SEM_KEY + 1;
const int SHMSZ = 27;

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

  if (allocator.remove () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "allocator.remove"));
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
main (void)
{
  char *shm = (char *) allocator.malloc (27);

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
int main (void)
{
  ACE_ERROR ((LM_ERROR, 
	      "SYSV IPC is not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_SYSV_IPC */

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Guard<ACE_SV_Semaphore_Simple>;
template class ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple>;
template class ACE_Read_Guard<ACE_SV_Semaphore_Simple>;
template class ACE_Write_Guard<ACE_SV_Semaphore_Simple>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
