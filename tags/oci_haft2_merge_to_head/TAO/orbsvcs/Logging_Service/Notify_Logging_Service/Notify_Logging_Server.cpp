//$Id$
#include "Notify_Logging_Service.h"

ACE_RCSID (Notify_Logging_Service,
           Notify_Logging_Server,
           "$Id$")

// Driver function for the TAO Notify Service.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Notify_Logging_Service notify_logging_service;

  ACE_DECLARE_NEW_CORBA_ENV;

  if (notify_logging_service.init (argc, argv ACE_ENV_ARG_PARAMETER) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Failed to start the Notification Logging Service.\n"),
                      1);

  ACE_TRY
    {
      notify_logging_service.run ();
      notify_logging_service.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      notify_logging_service.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Failed to start the Notification Logging Service\n");
      return 1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (1);

  return 0;
}
