// $Id$

#include "RateGenC.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"

/**
 * @file controller.cpp
 *
 * This program interact with a RateGen component, using its supported
 * interface "opmode" to switch it on/off, and set the rate of the
 * RateGen.
 */

const ACE_TCHAR *rategen_ior_ = 0;
int rate = 2;
int turn_on = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:r:of"));
  int c;

  while ((c = get_opts ()) != -1)
    {
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
          rate = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case '?':  // display help for use of the server.
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "usage:  %s\n"
                            "-o (Turn on the rate generator)\n"
                            "-f (Turn off the rate generator)\n"
                            "-k <RateGen IOR> (default is file://RateGen.ior)\n"
                            "-r <rate in hertz> (default is 3)\n"
                            "\n",
                            argv [0]),
                            -1);
          break;
        }
    }

  if (rategen_ior_ == 0)
    {
      rategen_ior_ = ACE_TEXT("file://RateGen.ior");
    }

  if (rate == 0)
    {
      rate = 3;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        {
          return -1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (rategen_ior_);

      HUDisplay::RateGen_var pulser =
        HUDisplay::RateGen::_narrow (obj.in ());

      if (CORBA::is_nil (pulser.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to acquire 'RateGen' objref\n"),
                            -1);
        }

      if (turn_on)
        {
          pulser->hertz (rate);

          ACE_DEBUG ((LM_DEBUG, "Start up the Rate Generator\n"));

          pulser->start ();
        }
      else
        {
          pulser->stop ();

          ACE_DEBUG ((LM_DEBUG, "Rate Generator stopped\n"));
        }
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Who is the culprit\n");
      cerr << "Uncaught CORBA exception" << endl;
      return 1;
    }

  return 0;
}
