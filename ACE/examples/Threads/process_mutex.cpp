// $Id$

// This program tests ACE_Process_Mutexes.  To run it, open 3 or 4
// windows and run this program in each window...

#include "ace/OS_main.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Thread_Mutex.h"
#include "ace/Signal.h"
#include "ace/Log_Msg.h"
#include "ace/Process_Mutex.h"



#if defined (ACE_HAS_THREADS)

static sig_atomic_t done;

extern "C" void
handler (int)
{
  done = 1;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  const ACE_TCHAR *name = argc > 1 ? argv[1] : ACE_TEXT("hello");
  int iterations =  argc > 2 ? ACE_OS::atoi (argv[2]) : 100;

  ACE_Process_Mutex pm (name);

  // Register a signal handler.
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
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "ACE doesn't support support threads on this platform (yet)\n"),
                    -1);
}
#endif /* ACE_HAS_THREADS */
