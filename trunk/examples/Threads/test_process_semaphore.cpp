// This program tests ACE_Process_Semaphore.  To run it, open 3 
// $Id$

// or 4 windows and run this program in each window...

#include "ace/Log_Msg.h"
#include "ace/Synch.h"
#include "ace/Signal.h"

static sig_atomic_t done;

static void 
handler (int)
{
  done = 1;
}

int
main (int argc, char *argv[])
{
  char *name = argc == 1 ? "hello" : argv[1];

  ACE_Process_Semaphore pm (1, name);

  ACE_Sig_Action handle ((ACE_SignalHandler) handler, SIGINT);

  for (int i = 0; i < 100 && !done; i++)
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
