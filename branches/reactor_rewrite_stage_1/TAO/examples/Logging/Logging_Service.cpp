// $Id$

#include "Logging_Service_i.h"
#include "tao/debug.h"

ACE_RCSID(Logging_Service, Logging_Service, "$Id$")

int
main (int argc, char *argv[])
{
  // Set TAO_debug_level to 0 to ensure silent running unless
  // debugging is explicitly invoked
  TAO_debug_level = 0;

  Logger_Server log_server;

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      int ret = log_server.init (argc, argv TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (ret == -1)
        return 1;
      else
        {
          log_server.run (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
