// $Id$

#include "ace/OS.h"
#include "ace/Arg_Shifter.h"
#include "Trading_Service.h"

ACE_RCSID(Trading_Service, Trading_Service, "$Id$")

Trading_Shutdown::Trading_Shutdown (Trading_Service& trader)
  : trader_ (trader)
{
  if (this->shutdown_.register_handler (SIGINT,
                                        this) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT("%p\n"),
                ACE_LIB_TEXT("register_handler")));

#ifndef ACE_HAS_WINCE
  if (this->shutdown_.register_handler (SIGTERM,
                                        this) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT("%p\n"),
                ACE_LIB_TEXT("register_handler")));
#endif  // ACE_HAS_WINCE
}

int
Trading_Shutdown::handle_signal (int signum,
                                 siginfo_t *sinfo,
                                 ucontext_t *ucon)
{
  ACE_UNUSED_ARG (signum);
  ACE_UNUSED_ARG (sinfo);
  ACE_UNUSED_ARG (ucon);
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
Trading_Service::init (int argc,
                       ACE_TCHAR *argv[]
                       ACE_ENV_ARG_DECL_NOT_USED)
{
  int result_trader =
    this->trading_loader_.init (argc, argv);

  if (result_trader == -1)
    return -1;

  return 0;
}

int
Trading_Service::run (ACE_ENV_SINGLE_ARG_DECL)
{
  Trading_Shutdown trading_shutdown (*this);

  int return_value =
    this->trading_loader_.run (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return return_value;
}

int
Trading_Service::shutdown (void)
{
  // Invoke TAO_Trading_Loader::fini ()
  int shutdown_result =
        this->trading_loader_.fini ();

  if (shutdown_result == -1)
    return -1;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
