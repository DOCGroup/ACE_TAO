// $Id$
#include "../Simple_util.h"
#include "AccountManager_i.h"

// This is the main driver program for the time and date server.

int
main (int argc, char *argv[])
{
  Server<AccountManager_i> server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tBank server\n\n"));

  ACE_TRY_NEW_ENV
    {
      if (server.init ("Bank",
                       argc,
                       argv
                       ACE_ENV_ARG_PARAMETER) == -1)
        return 1;
      else
        {
          server.run ();
        }
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex,"User Exception");
      return -1;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex,"System Exception");
      return -1;
    }

  ACE_ENDTRY;

  return 0;
}

