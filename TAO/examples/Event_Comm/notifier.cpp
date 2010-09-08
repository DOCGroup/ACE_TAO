// $Id$

#include "Notifier_Server.h"
#include "notifier.h"

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
  try
    {
      ns_.run ();
    }
  catch (const CORBA::Exception&)
    {
      return;
    }
}

Notifier::Notifier (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ns_.init (argc, argv);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Notifier_Server.init failed\n");
    }

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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // Initialize server daemon.
  Notifier notifier (argc, argv);

  // Loop forever handling events.
  notifier.run ();

  return 0;
}
