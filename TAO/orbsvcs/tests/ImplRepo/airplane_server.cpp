// $Id$

#include "airplane_server_i.h"

ACE_RCSID(ImplRepo, airplane_server, "$Id$")

int
main (int argc, char *argv[])
{
  Airplane_Server_i server;

  ACE_DEBUG ((LM_DEBUG, "\n\tPaper Airplane Server\n\n"));
  TAO_TRY
    {
      if (server.init (argc, argv, TAO_TRY_ENV) == -1)
        return -1;
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
