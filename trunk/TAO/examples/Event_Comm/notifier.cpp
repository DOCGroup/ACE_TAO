// $Id$

#include "Notifier_Server.h"

ACE_RCSID(Notifier, notifier, "$Id$")

class Notifier : public ACE_Event_Handler
{
  // = TITLE
  //   Notifier driver for the TAO Publish/Subscribe example.
  //
  // = DESCRIPTION
  //    The driver class for the <Event_Comm::Notifier> object.
public:
  // = Initialization and termination methods.
  Notifier (int argc, char *argv[]);
  // Constructor.

  ~Notifier (void);
  // Destructor.

  void run (void);
  // Execute the notifier.

private:
  virtual int handle_signal (int signum,
			     siginfo_t *,
			     ucontext_t *);
  // Handle signals that shut us down.

  Notifier_Server ns_;
  // The notifier server.
};

int
Notifier::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG,
              "got signal in handle_signal %S\n",
              signum));

  // tell the <Notifier_Server> to shut down the ORB.
  ns_.close ();
  return 0;
}

void
Notifier::run (void)
{
  TAO_TRY
    {
      ns_.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
    }
  TAO_ENDTRY;
}

Notifier::Notifier (int argc, char *argv[])
{
  TAO_TRY
    {
      ns_.init (argc, argv, TAO_TRY_ENV);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Notifier_Server.init failed\n ");
    }
  TAO_ENDTRY;

  // Register with the ORB's Reactor to receive a signal to shut us
  // down.
  if (ns_.reactor ()->register_handler (SIGINT, this) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "register_handler"));
}

Notifier::~Notifier (void)
{
  // cleanup.
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
