// $Id$

#include "PP_Test_Server.h"
#include "tao/Timeprobe.h"

ACE_RCSID(IDL_Cubit, server, "$Id$")

// This runs the server test.

int
main (int argc, char *argv[])
{
  PP_Test_Server pp_test_server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tPluggable_Test: server\n\n"));
  TAO_TRY
    {
      int status = pp_test_server.init (argc, argv, TAO_TRY_ENV);

      if (status == -1)
        {
          TAO_TRY_ENV.print_exception ("Initialization Exception");
          return -1;
        }
      else
        {
          pp_test_server.run (TAO_TRY_ENV);
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
