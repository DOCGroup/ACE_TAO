// $Id$

#include "Logging_Service_i.h"

ACE_RCSID(Logging_Service, Logging_Service, "$Id$")

int
main (int argc, char *argv[])
{
  // Set TAO_debug_level to 0 to ensure silent running unless
  // debugging is explicitly invoked
  TAO_debug_level = 0;
  
  Logger_Server log_server; 

   TAO_TRY
    {
      if (log_server.init (argc, argv, TAO_TRY_ENV) == -1)
        return 1;
      else
        {
          log_server.run (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}


