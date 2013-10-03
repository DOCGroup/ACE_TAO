// $Id$

#include "orbsvcs/Log_Macros.h"
#include "ace/Arg_Shifter.h"
#include "Trading_Service.h"

Trading_Shutdown::Trading_Shutdown (Trading_Service& trader)
  : trader_ (trader)
{
  if (this->shutdown_.register_handler (SIGINT,
                                        this) == -1)
    ORBSVCS_ERROR ((LM_ERROR,
                ACE_TEXT("%p\n"),
                ACE_TEXT("register_handler")));

#ifndef ACE_HAS_WINCE
  if (this->shutdown_.register_handler (SIGTERM,
                                        this) == -1)
    ORBSVCS_ERROR ((LM_ERROR,
                ACE_TEXT("%p\n"),
                ACE_TEXT("register_handler")));
#endif  // ACE_HAS_WINCE
}

int
Trading_Shutdown::handle_signal (int,
                                 siginfo_t *,
                                 ucontext_t *)
{
  this->trader_.~Trading_Service ();
  ACE_OS::exit (0);
  return 0;
}

Trading_Service::Trading_Service (void)
{
  // constructor
}

Trading_Service::~Trading_Service (void)
{
  this->shutdown ();
}

int
Trading_Service::init (int argc, ACE_TCHAR *argv[])
{
  int const result_trader = this->trading_loader_.init (argc, argv);

  if (result_trader == -1)
    return -1;

  return 0;
}

int
Trading_Service::run (void)
{
  Trading_Shutdown trading_shutdown (*this);

  return this->trading_loader_.run ();
}

int
Trading_Service::shutdown (void)
{
  // Invoke TAO_Trading_Loader::fini ()
  int const shutdown_result = this->trading_loader_.fini ();

  if (shutdown_result == -1)
    return -1;

  return 0;
}
