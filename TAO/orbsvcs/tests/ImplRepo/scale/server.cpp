// $Id$

#include "server_i.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      Server_i server;

      int retval = server.init (convert.get_argc(), convert.get_ASCII_argv() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval == -1)
        return -1;
      else
        {
          server.run (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "Server: System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "Server: User Exception");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
