// $Id$

#include "Trading_Service.h"

// Driver function for the TAO Trading Service.

int
main (int argc, char *argv[])
{
  Trading_Service trader;

  ACE_TRY_NEW_ENV
    {
      int check =
        trader.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (check != -1)
        {
          trader.run (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failed to initialize the trader.\n"),
                          1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Trading Service");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
