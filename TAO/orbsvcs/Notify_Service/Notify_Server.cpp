// $Id$

#include "Notify_Service.h"

TAO_Notify_Service notify_service;

extern "C" void handler (int signum)
{
  // check of sigint
  if (signum == SIGINT)
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      notify_service.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
}

// Driver function for the TAO Notify Service.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);

  // Init factories.

  ACE_DECLARE_NEW_CORBA_ENV;

  if (notify_service.init (argc, argv ACE_ENV_ARG_PARAMETER) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT("Failed to initialize the Notification Service.\n")),
                      1);

  ACE_TRY
    {
      notify_service.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_LIB_TEXT("Failed to run the Notification Service\n"));
      return 1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (1);

  return 0;
}
