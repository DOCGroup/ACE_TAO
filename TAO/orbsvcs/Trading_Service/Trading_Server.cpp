#include "orbsvcs/Log_Macros.h"
#include "Trading_Service.h"
#include "ace/OS_main.h"

// Driver function for the TAO Trading Service.
int
ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  Trading_Service trader;

  try
    {
      int const result = trader.init (argc, argv);
      if (result == -1)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           "Failed to initialize the trader.\n"),
                          1);

      trader.run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Trading Service");
      return 1;
    }

  return 0;
}
