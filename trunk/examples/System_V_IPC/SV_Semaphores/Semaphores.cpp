// Illustrates the use of the Semaphore_Complex class.  Note that it
// @(#)Semaphores.cpp	1.1	10/18/96

// doesn't matter whether the parent or the child creates the
// semaphore since Semaphore_Complex will correctly serialize the
// intialization of the mutex and synch objects.
#include "ace/Malloc.h"
#include "ace/SV_Semaphore_Complex.h"
#include "Semaphore_Test.h"

ACE_Malloc<ACE_Shared_Memory_Pool, ACE_SV_Semaphore_Simple> allocator;
ACE_SV_Semaphore_Complex *mutex = 0; 
ACE_SV_Semaphore_Complex *synch = 0; 

/* Pointer to memory shared by both the client and server. */
static char *shm;

static int
do_parent (void)
{
  char *s = shm;

  mutex = new ACE_SV_Semaphore_Complex (SEM_KEY_1, ACE_SV_Semaphore_Complex::ACE_CREATE, 0);
  synch = new ACE_SV_Semaphore_Complex (SEM_KEY_2, ACE_SV_Semaphore_Complex::ACE_CREATE, 0);

  for (char c = 'a'; c <= 'z'; c++)
    *s++ = c;

  *s = '\0';

  if (mutex->release () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p", "server mutex.release"), 1);

  if (synch->acquire () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p", "server synch.acquire"), 1);
  return 0;
}

static int
do_child (void)
{
  mutex = new ACE_SV_Semaphore_Complex (SEM_KEY_1, ACE_SV_Semaphore_Complex::ACE_CREATE, 0);
  synch = new ACE_SV_Semaphore_Complex (SEM_KEY_2, ACE_SV_Semaphore_Complex::ACE_CREATE, 0);

  while (mutex->tryacquire () == -1)
    if (errno == EAGAIN)
      ACE_DEBUG ((LM_DEBUG, "spinning in client!\n"));
    else
      ACE_ERROR_RETURN ((LM_ERROR, "client mutex.tryacquire"), 1);

  for (char *s = (char *) shm; *s != '\0'; s++)
    putchar (*s);

  putchar ('\n');

  if (synch->release () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "client synch.release"), 1);
  return 0;
}

int
main (void)
{
  shm = (char *) allocator.malloc (27);

  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "fork failed\n"), -1);
      /* NOTREACHED */
    case 0:
      return do_child ();
    default:
      {
	int result = do_parent ();

	if (wait (0) == -1)
	  ACE_ERROR_RETURN ((LM_ERROR, "wait"), -1);

	allocator.remove ();

	if (mutex->remove () == -1)
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "mutex.remove"), -1);
	else if (synch->remove () == -1)
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "synch.remove"), -1);
	return result;
      }
    }
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Malloc<ACE_Shared_Memory_Pool, ACE_SV_Semaphore_Simple>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

