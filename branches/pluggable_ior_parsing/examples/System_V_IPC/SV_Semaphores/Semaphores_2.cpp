// $Id$

// Illustrates the use of the ACE_SV_Semaphore_Complex class and the
// ACE_Malloc class using the ACE_Shared_Memory_Pool (which uses
// System V shared memory).  Note that it doesn't matter whether the
// parent or the child creates the semaphore since Semaphore_Complex
// will correctly serialize the intialization of the mutex and synch
// objects.

#include "ace/Malloc.h"
#include "ace/SV_Semaphore_Complex.h"

ACE_RCSID(SV_Semaphores, Semaphores_2, "$Id$")

#if defined (ACE_HAS_SYSV_IPC) && !defined(ACE_LACKS_SYSV_SHMEM)

// Shared memory allocator (note that this chews up the
// ACE_DEFAULT_SEM_KEY).
static ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple> my_alloc;

const int SEM_KEY_1 = ACE_DEFAULT_SEM_KEY + 1;
const int SEM_KEY_2 = ACE_DEFAULT_SEM_KEY + 2;

static int
parent (char *shm)
{
  char *s = shm;

  // Both semaphores are initially created with a count of 0, i.e.,
  // they are "locked."
  ACE_SV_Semaphore_Complex mutex (SEM_KEY_1, ACE_SV_Semaphore_Complex::ACE_CREATE, 0);
  ACE_SV_Semaphore_Complex synch (SEM_KEY_2, ACE_SV_Semaphore_Complex::ACE_CREATE, 0);

  // This is a critical section, which is protected by the mutex
  // semaphore.
  for (char c = 'a'; c <= 'z'; c++)
    *s++ = c;

  *s = '\0';

  if (mutex.release () == -1)
    ACE_ERROR ((LM_ERROR, "%p", "parent mutex.release"));
  else if (synch.acquire () == -1)
    ACE_ERROR ((LM_ERROR, "%p", "parent synch.acquire"));

  if (my_alloc.remove () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "my_alloc.remove"));
  if (mutex.remove () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "mutex.remove"));
  if (synch.remove () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "synch.remove"));
  return 0;
}

static int
child (char *shm)
{
  // Both semaphores are initially created with a count of 0, i.e.,
  // they are "locked."
  ACE_SV_Semaphore_Complex mutex (SEM_KEY_1, ACE_SV_Semaphore_Complex::ACE_CREATE, 0);
  ACE_SV_Semaphore_Complex synch (SEM_KEY_2, ACE_SV_Semaphore_Complex::ACE_CREATE, 0);

  // Perform "busy waiting" here until we acquire the semaphore.  This
  // isn't really a good design -- it's just to illustrate that you
  // can do non-blocking acquire() calls with the ACE System V
  // semaphore wrappers.
  while (mutex.tryacquire () == -1)
    if (errno == EAGAIN)
      ACE_DEBUG ((LM_DEBUG, "spinning in child!\n"));
    else
      ACE_ERROR_RETURN ((LM_ERROR, "child mutex.tryacquire"), 1);

  for (char *s = (char *) shm; *s != '\0'; s++)
    ACE_DEBUG ((LM_DEBUG, "%c", *s));

  ACE_DEBUG ((LM_DEBUG, "\n"));

  if (synch.release () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "child synch.release"), 1);
  return 0;
}

int
main (int, char *[])
{
  char *shm = (char *) my_alloc.malloc (27);

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
int main (int, char *[])
{
  ACE_ERROR ((LM_ERROR,
              "SYSV IPC, or SYSV SHMEM is not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_SYSV_IPC */

#if defined (ACE_HAS_SYSV_IPC) && !defined(ACE_LACKS_SYSV_SHMEM)

# if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Guard<ACE_SV_Semaphore_Simple>;
template class ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple>;
template class ACE_Malloc_T<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple, ACE_Control_Block>;
template class ACE_Read_Guard<ACE_SV_Semaphore_Simple>;
template class ACE_Write_Guard<ACE_SV_Semaphore_Simple>;
# elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Guard<ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Malloc<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Malloc_T<ACE_SHARED_MEMORY_POOL, ACE_SV_Semaphore_Simple, ACE_Control_Block>
#pragma instantiate ACE_Read_Guard<ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Write_Guard<ACE_SV_Semaphore_Simple>
# endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_SYSV_IPC */
