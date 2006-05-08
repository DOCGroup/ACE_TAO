// $Id$

#include "Notifier_Server.h"
#include "notifier.h"
#include "ace/Argv_Type_Converter.h"
ACE_RCSID(Notifier, notifier, "$Id$")


int
Notifier::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG,
              "got signal in handle_signal %S\n",
              signum));

  // Tell the <Notifier_Server> to shut down the ORB.
  ns_.close ();
  return 0;
}

void
Notifier::run (void)
{
  ACE_TRY_NEW_ENV
    {
      ns_.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return;
    }
  ACE_ENDTRY;
}

Notifier::Notifier (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      ns_.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Notifier_Server.init failed\n ");
    }
  ACE_ENDTRY;

  // Register with the ORB's Reactor to receive a signal to shut us
  // down.
  if (ns_.reactor ()->register_handler (SIGINT, this) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "register_handler"));
}

Notifier::~Notifier (void)
{
  // Cleanup.
  this->ns_.close ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);
  // Initialize server daemon.
  Notifier notifier (convert.get_argc(), convert.get_ASCII_argv());

  // Loop forever handling events.
  notifier.run ();

  return 0;
}
