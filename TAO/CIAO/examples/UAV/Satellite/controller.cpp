// $Id$
/**
 * @file controller.cpp
 *
 * This program initiates a satellite alert.
 */


#include "SatelliteC.h"
#include "ace/Get_Opt.h"
#include <iostream>

char *satellite_ior_ = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:r:of");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {

      case 'k':
        satellite_ior_ = get_opts.opt_arg ();
        break;

      break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-k <Satellite IOR> (default is file://sat.ior)\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (satellite_ior_ == 0)
    satellite_ior_ = "file://sat.ior";

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv ACE_ENV_ARG_PARAMETER);

      if (parse_args (argc, argv) != 0)
        return -1;

      CORBA::Object_var obj
        = orb->string_to_object (satellite_ior_
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      BBN_UAV::Satellite_var sat
        = BBN_UAV::Satellite::_narrow (obj.in ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (sat.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire 'Satellite' objref\n"), -1);

      ACE_DEBUG ((LM_DEBUG, "Send a satellite alert\n"));

      for (int i = 0; i < 1001; ++i)
        {
          sat->alert (ACE_ENV_SINGLE_ARG_PARAMETER);
	  std::cerr << "Pushed fine... " << std::endl;
          ACE_TRY_CHECK;
        }

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Who is the culprit \n");
      std::cerr << "Uncaught CORBA exception" << std::endl;
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
