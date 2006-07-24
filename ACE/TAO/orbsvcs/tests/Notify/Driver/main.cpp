//$Id$

#include "../lib/Driver.h"
#include "ace/OS_main.h"

int
ACE_TMAIN (int argc, char *argv[])
{
  TAO_Notify_Tests_Driver driver;

  ACE_TRY_NEW_ENV
    {
      int result = driver.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("Failed to initialize the Notify Testing Driver.\n")),
                            1);

      driver.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, ACE_TEXT("Error: \n"));
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
