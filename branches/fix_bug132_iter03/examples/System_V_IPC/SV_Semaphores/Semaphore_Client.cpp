#include "Semaphore_Test.h"
// $Id$

#include "ace/SV_Shared_Memory.h"
#include "ace/SV_Semaphore_Simple.h"

int
main (void)
{
  ACE_SV_Shared_Memory shm_client (SHM_KEY, 
				   SHMSZ, 
				   ACE_SV_Shared_Memory::ACE_OPEN);
  ACE_SV_Semaphore_Simple sem (SEM_KEY_1, 
			       ACE_SV_Semaphore_Simple::ACE_OPEN, 0, 2);

  char *s = (char *) shm_client.get_segment_ptr ();

  if (sem.acquire (0) < 0)
    ACE_OS::perror ("client sem.acquire"), ACE_OS::exit (1);

  while (*s != '\0')
    putchar (*s++);

  putchar ('\n');

  if (sem.release (1) < 0)
    ACE_OS::perror ("client sem.release"), ACE_OS::exit (1);

  return 0;
}

