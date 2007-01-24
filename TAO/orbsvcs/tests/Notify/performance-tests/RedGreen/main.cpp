// -*- C++ -*-
// $Id$

#include "RedGreen_Test.h"

int
main (int argc, char *argv [])
{
  ACE_High_Res_Timer::calibrate ();

  RedGreen_Test client;

  client.parse_args (argc, argv);

  ACE_TRY_NEW_ENV
    {
      client.init (argc, argv
                   ACE_ENV_ARG_PARAMETER); //Init the Client

      client.run ();

      client.destroy_ec ();
    }
  ACE_CATCH (CORBA::Exception, se)
    {
      ACE_PRINT_EXCEPTION (se, "Error: ");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
