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

  ACE_DECLARE_NEW_CORBA_ENV;
   ACE_TRY
    {
      if (log_server.init (argc, argv, ACE_TRY_ENV) == -1)
        return 1;
      else
        {
          log_server.run (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      ACE_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      ACE_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}


