// $Id$

// This program tests ACE_Process_Semaphore.  To run it, open 3 or 4
// windows and run this program in each window...

#include "ace/Synch.h"
#include "ace/Signal.h"

ACE_RCSID(Threads, process_semaphore, "$Id$")

static sig_atomic_t done;

extern "C" void
handler (int)
{
  done = 1;
}

int
main (int argc, char *argv[])
{
  const char *name = argc == 1 ? "hello" : argv[1];
  int iterations =  argc > 2 ? ACE_OS::atoi (argv[2]) : 100;

  ACE_Process_Semaphore pm (1, name);

  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  for (int i = 0; i < iterations && !done; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = acquiring\n"));
      if (pm.acquire () == -1)
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) = %p\n", "acquire failed"));
      else
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) = acquired\n"));

      ACE_OS::sleep (3);

      if (pm.release () == -1)
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) = %p\n", "release failed"));
      else
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) = released\n"));

      if (pm.tryacquire () == -1)
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) = %p\n", "tryacquire failed"));
      else
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) = tryacquire\n"));

      if (pm.release () == -1)
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) = %p\n", "release failed"));
      else
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) = released\n"));
    }

  if (argc > 2)
    pm.remove ();
  return 0;
}

