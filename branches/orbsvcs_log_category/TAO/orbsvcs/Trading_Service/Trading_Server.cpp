// $Id$

#include "orbsvcs/Log_Macros.h"
#include "Trading_Service.h"
#include "ace/OS_main.h"

// Driver function for the TAO Trading Service.

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  Trading_Service trader;

  try
    {
      int check = trader.init (argc, argv);

      if (check != -1)
        {
          trader.run ();
        }
      else
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           "Failed to initialize the trader.\n"),
                          1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Trading Service");
      return 1;
    }

  return 0;
}
