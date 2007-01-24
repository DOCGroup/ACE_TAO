// $Id$
#include "../Simple_util.h"
#include "Grid_i.h"

// This is the main driver program for the time and date server.

int
main (int argc, char *argv[])
{
  Server<Grid_Factory_i> server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tGrid server\n\n"));

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      if (server.init ("Grid",
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
      //ACE_UNUSED_ARG (userex);
      ACE_PRINT_EXCEPTION (userex, "User Exception in main");
      return -1;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      //ACE_UNUSED_ARG (sysex);
      ACE_PRINT_EXCEPTION (sysex, "System Exception in main ");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

