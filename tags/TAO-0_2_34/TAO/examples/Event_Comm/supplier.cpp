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
  //    This class starts up the <Supplier_Input_Handler> and
  //    <Notifier_Handler> objects.
public:
  // Initialization and Termination methods.
  Supplier (void);
  // Constructor.

  ~Supplier (void);
  // Destructor.

  int init (int argc, char *argv[]);
  // Initialization method. returns 0 on success, -1 on error.

  void run (void);
  // Execute the supplier.

private:
  virtual int handle_signal (int signum,
			     siginfo_t *,
			     ucontext_t *);
  // Handle shutdown signals.

  Supplier_Input_Handler ih_;
  // Handler for keyboard input.

  Notifier_Handler nh_;
  // The notifier handler.
};

Supplier::Supplier (void)
{
  // No-Op.
}

Supplier::~Supplier (void)
{
  // No-Op.
}

int
Supplier::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG,
              "%S\n",
              signum));

  this->ih_.handle_close ();
  this->nh_.handle_close ();

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

int
Supplier::init (int argc, char *argv[])
{
  if (this->nh_.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "Notifier_Handler did not init\n"), -1);

   if (this->ih_.initialize (&nh_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "Supplier Input handler did not init\n"), -1);

  if (ACE_Reactor::instance ()->register_handler (SIGINT,
						  this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "register_handler"), -1);
}

int
main (int argc, char *argv[])
{
  // Initialize server daemon.
  Supplier supplier;

  if (supplier.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                "%p\n",
                "supplier init failed"), 1);

  // Loop forever handling events.
  supplier.run ();

  return 0;
}
