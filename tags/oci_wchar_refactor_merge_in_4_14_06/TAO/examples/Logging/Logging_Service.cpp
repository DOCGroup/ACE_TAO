// $Id$

#include "Logging_Service_i.h"
#include "tao/debug.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID(Logging_Service, Logging_Service, "$Id$")

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);
  // Set TAO_debug_level to 0 to ensure silent running unless
  // debugging is explicitly invoked
  TAO_debug_level = 0;

  Logger_Server log_server;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int ret = log_server.init (convert.get_argc(), convert.get_ASCII_argv() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (ret == -1)
        return 1;
      else
        {
          log_server.run (ACE_ENV_SINGLE_ARG_PARAMETER);
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
