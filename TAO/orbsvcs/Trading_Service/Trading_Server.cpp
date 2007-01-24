// $Id$

#include "Trading_Service.h"
#include "ace/OS_main.h"

// Driver function for the TAO Trading Service.

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  Trading_Service trader;

  ACE_TRY_NEW_ENV
    {
      int check =
        trader.init (argc, argv ACE_ENV_ARG_PARAMETER);

      if (check != -1)
        {
          trader.run ();
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
