// $Id$

#include "Naming_Service.h"

// Driver function for the TAO Naming Service.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO_Naming_Service naming_service;

  if (naming_service.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT("Failed to start the Naming Service.\n")),
                      1);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      naming_service.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "NamingService");
      return 1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (1);

  return 0;
}
