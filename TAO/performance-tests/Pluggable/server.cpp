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
  ACE_TRY_NEW_ENV
    {
      int status = pp_test_server.init (argc, argv, ACE_TRY_ENV);

      if (status == -1)
        {
          ACE_TRY_ENV.print_exception ("Initialization Exception");
          return -1;
        }
      else
        {
          pp_test_server.run (ACE_TRY_ENV);
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
