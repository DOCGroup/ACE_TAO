// $Id$

#include "Notifier_Handler.h"

ACE_RCSID(Notifier, notifier, "$Id$")

class Notifier : public ACE_Event_Handler
{
  // = TITLE
  //   Notifier driver for the TAO Publish/Subscribe example.
  //
  // = DESCRIPTION
  //   The executable file generated from this code should be
  //   registered (under the name 'logger').
public:
  Notifier (int argc, char *argv[]);
  ~Notifier (void);

  void run (void);
  // Execute the notifier.

private:
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);

  Notifier_Handler *nh_;
  // Handler for CORBA Notifier.
};

int
Notifier::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
              "closing down Notifier\n"));
  return 0;
}

int
Notifier::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG,
              "%S\n",
              signum));
  ACE_Reactor::end_event_loop ();
  return 0;
}

void
Notifier::run (void)
{
  if (ACE_Reactor::run_event_loop () == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "run_reactor_event_loop"));
}

Notifier::Notifier (int argc, char *argv[])
  : ih_ (0),
    nh_ (0)
{
  ACE_DEBUG ((LM_DEBUG,
	      "no config file, using static binding\n"));

  // Pass in program exec name to use a service_location!
  ACE_NEW (this->nh_,
	   Notifier_Handler (argv[0],
			     "notifier"));
  ACE_NEW (this->ih_,
	   Notifier_Input_Handler (this->nh_));

  if (ACE_Reactor::instance ()->register_handler (SIGINT, this) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "register_handler"));
}

Notifier::~Notifier (void)
{
  // Free up the handlers if they were statically bound.
  this->nh_->handle_close ();
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
