// $Id$

#include "Cubit_Server.h"
#include "tao/Timeprobe.h"

ACE_RCSID(IDL_Cubit, server, "$Id$")

// This runs the server test.

int
main (int argc, char *argv[])
{
  Cubit_Server cubit_server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_Cubit: server\n\n"));
  ACE_TRY_NEW_ENV
    {
      if (cubit_server.init (argc, argv, ACE_TRY_ENV) == -1)
        {
          ACE_TRY_ENV.print_exception ("Initialization Exception");
          return -1;
        }
      else
        {
          cubit_server.run (ACE_TRY_ENV);
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
  ACE_TIMEPROBE_PRINT;
  return 0;
}
