// $Id$

#include "Notifier_Server.h"
#include "notifier.h"
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
      ns_.run (ACE_TRY_ENV);
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
      ns_.init (argc, argv, ACE_TRY_ENV);
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("Notifier_Server.init failed\n ");
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
main (int argc, char *argv[])
{
  // Initialize server daemon.
  Notifier notifier (argc, argv);

  // Loop forever handling events.
  notifier.run ();

  return 0;
}
