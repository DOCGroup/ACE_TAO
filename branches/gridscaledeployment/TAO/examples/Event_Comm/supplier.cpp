// $Id$

#include "Notifier_Handler.h"
#include "Supplier_Input_Handler.h"
#include "supplier.h"

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

  this->close ();
  return 0;
}

void
Supplier::run (void)
{
  if (nh_.run () == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "Notifier_Handler::run"));
}

void
Supplier::close (void)
{
  ih_.close ();
  nh_.close ();
}

int
Supplier::init (int argc, ACE_TCHAR *argv[])
{
  if (this->nh_.init (argc, argv, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "Notifier_Handler did not init\n"),
                      -1);
  else if (this->ih_.initialize (&nh_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "Supplier Input handler did not init\n"),
                      -1);
  else if (nh_.reactor ()->register_handler (SIGINT, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "register_handler"),
                      -1);
  ACE_DEBUG ((LM_DEBUG,
              "Supplier initialized..\n"));
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // Initialize server daemon.
  Supplier supplier;

  try
    {

      if (supplier.init (argc, argv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                          "%p\n",
                          "supplier init failed"),
                          1);

      // Loop forever handling events.
      supplier.run ();
    }
  catch (const ::CORBA::Exception &e)
    {
      e._tao_print_exception ("Caught unexpected CORBA exception : ");
      return 1;
    }

  return 0;
}
