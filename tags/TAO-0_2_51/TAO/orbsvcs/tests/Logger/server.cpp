// $Id$

#include "server_i.h"

ACE_RCSID(Logger, server, "$Id$")

int
main (int argc, char *argv[])
{
  Logger_Server log_server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_Logger:SERVER\n\n"));

   TAO_TRY
    {
      if (log_server.init (argc, argv, TAO_TRY_ENV) == -1)
        return 1;
      else
        {
          log_server.run (TAO_TRY_ENV);
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


