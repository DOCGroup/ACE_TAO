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
  TAO_TRY
    {
      if (cubit_server.init (argc, argv, TAO_TRY_ENV) == -1)
        {
          TAO_TRY_ENV.print_exception ("Initialization Exception");
          return -1;
        }
      else
        {
          cubit_server.run (TAO_TRY_ENV);
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
  ACE_TIMEPROBE_PRINT;
  return 0;
}
