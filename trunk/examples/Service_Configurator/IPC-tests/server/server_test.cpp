/* The main test driver for the dynamically configured server. */
// $Id$


#include "ace/Service_Config.h"

sig_atomic_t finished = 0;

static void
handler (int)
{
  finished = 1;
}

int
main (int argc, char *argv[])
{
  ACE_Service_Config server_test;

  ACE_Sig_Action sig ((ACE_SignalHandler) handler, SIGINT);

  if (server_test.open (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "server_test.open"), -1);

  for (;;)
    if (server_test.run_reactor_event_loop () == -1 && finished)
      break;

  /* NOTREACHED */
  return 0;
}
