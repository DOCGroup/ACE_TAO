// $Id$

#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Log_Msg.h"
// Listing 1 code/ch11
#include "ace/Signal.h"

// Forward declaration.
static void register_actions ();

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_TRACE ("::main");

  ::register_actions ();    // Register actions to happen.

  // This will be raised immediately.
  ACE_OS::kill (ACE_OS::getpid(), SIGUSR2);

  // This will pend until the first signal is completely
  // handled and returns, because we masked it out
  // in the registerAction call.
  ACE_OS::kill (ACE_OS::getpid (), SIGUSR1);

  while (ACE_OS::sleep (100) == -1)
    {
      if (errno == EINTR)
        continue;
      else
        ACE_OS::exit (1);
    }
  return 0;
}
// Listing 1
#if defined (ACE_HAS_SIG_C_FUNC)
extern "C" {
#endif
// Listing 3 code/ch11
static void my_sighandler (int signo)
{
  ACE_TRACE ("::my_sighandler");

  ACE_OS::kill (ACE_OS::getpid (), SIGUSR1);

  if (signo == SIGUSR1)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Signal SIGUSR1\n")));
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Signal SIGUSR2\n")));

  ACE_OS::sleep (10);
}
#if defined (ACE_HAS_SIG_C_FUNC)
}
#endif
// Listing 3
// Listing 2 code/ch11
static void register_actions ()
{
  ACE_TRACE ("::register_actions");

  ACE_Sig_Action sa (reinterpret_cast <ACE_SignalHandler> (my_sighandler));

  // Make sure we specify that SIGUSR1 will be masked out
  // during the signal handler's execution.
  ACE_Sig_Set ss;
  ss.sig_add (SIGUSR1);
  sa.mask (ss);

  // Register the same handler function for these
  // two signals.
  sa.register_action (SIGUSR1);
  sa.register_action (SIGUSR2);
}
// Listing 2
