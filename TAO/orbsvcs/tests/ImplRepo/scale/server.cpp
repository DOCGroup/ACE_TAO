// $Id$

#include "server_i.h"

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      Server_i server;

      int retval = server.init (argc, argv ACE_ENV_ARG_PARAMETER);

      if (retval == -1)
        return -1;
      else
        {
          server.run ();
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
