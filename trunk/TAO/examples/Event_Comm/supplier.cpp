// $Id$

#include "Notifier_Handler.h"
#include "Supplier_Input_Handler.h"

ACE_RCSID(Supplier, supplier, "$Id$")

class Supplier : public ACE_Event_Handler
{
  // = TITLE
  //   Supplier driver for the TAO Publish/Subscribe example.
  //
  // = DESCRIPTION
  //   The executable file generated from this code should be
  //   registered (under the name 'logger').
public:
  Supplier (int argc, char *argv[]);
  ~Supplier (void);

  void run (void);
  // Execute the supplier.

private:
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);

  Supplier_Input_Handler *ih_;
  // Handler for keyboard input.
};

int
Supplier::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
              "closing down Supplier\n"));
  return 0;
}

int
Supplier::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG,
              "%S\n",
              signum));
  ACE_Reactor::end_event_loop ();
  return 0;
}

void
Supplier::run (void)
{
  if (ACE_Reactor::run_event_loop () == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "run_reactor_event_loop"));
}

Supplier::Supplier (int argc, char *argv[])
  : ih_ (0)
{
  ACE_DEBUG ((LM_DEBUG,
	      "no config file, using static binding\n"));

  ACE_NEW (this->ih_,
	   Supplier_Input_Handler (this->nh_));

  if (ACE_Reactor::instance ()->register_handler (SIGINT, this) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "register_handler"));
}

Supplier::~Supplier (void)
{
  // Free up the handlers if they were statically bound.
  this->ih_->handle_close ();
}

int
main (int argc, char *argv[])
{
  // Initialize server daemon.
  Supplier supplier (argc, argv);

  // Loop forever handling events.
  supplier.run ();

  return 0;
}
