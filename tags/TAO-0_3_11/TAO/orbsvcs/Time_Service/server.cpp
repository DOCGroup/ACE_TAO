// $Id$

#include "Server_i.h"

ACE_RCSID(TimeService, server, "$Id$")

// This is the main driver program for the Time Service server.

int
main (int argc, char *argv[])
{
  Server_i server;

  ACE_DEBUG ((LM_DEBUG,
              "[SERVER] Process/Thread Id : (%P/%t) Time Service server\n"));

  TAO_TRY
    {
      if (server.init (argc, argv, TAO_TRY_ENV) == -1)
        return 1;
      else
        {
          server.run (TAO_TRY_ENV);
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

  return 0;
}
