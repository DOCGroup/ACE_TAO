// $Id$

#include "RateGenC.h"
#include "ace/Get_Opt.h"

/**
 * @file controller.cpp
 *
 * This program interact with a RateGen component, using its supported
 * interface "opmode" to switch it on/off, and set the rate of the
 * RateGen.
 */

char *rategen_ior_ = 0;
int rate = 2;
int turn_on = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:r:of");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        turn_on = 1;
        break;

      case 'f':
        turn_on = 0;
        break;

      case 'k':
        rategen_ior_ = get_opts.opt_arg ();
        break;

      case 'r':
       rate = atoi (get_opts.opt_arg ());
      break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-o (Turn on the rate generator)\n"
                           "-f (Turn off the rate generator)\n"
                           "-k <RateGen IOR> (default is file://rategen.ior)\n"
                           "-r <rate in hertz> (default is 3)\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (rategen_ior_ == 0)
    rategen_ior_ = "file://rategen.ior";

  if (rate == 0)
    rate = 3;

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
        = orb->string_to_object (rategen_ior_
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      HUDisplay::RateGen_var pulser
        = HUDisplay::RateGen::_narrow (obj.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (pulser.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire 'RateGen' objref\n"), -1);

      if (turn_on)
        {
          pulser->hertz (rate
                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG, "Start up the Rate Generator\n"));

          pulser->start (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        {
          pulser->stop (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG, "Rate Generator stopped\n"));
        }

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Who is the culprit \n");
      cerr << "Uncaught CORBA exception" << endl;
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
