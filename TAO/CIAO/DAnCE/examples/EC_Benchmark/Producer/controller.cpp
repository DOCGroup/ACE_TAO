// $Id$

/**
 * @file controller.cpp
 *
 * This program interact with a Producer component.
 */

#include "ProducerC.h"
#include "ace/Get_Opt.h"

char* producer_ior = 0;

int niterations = 100;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:r:of");
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          producer_ior = get_opts.opt_arg ();
          break;

        case '?':  // display help for use of the server.
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage:  %s\n"
                             "-k <EC IOR> (default is file://Producer.ior)\n"
                             "\n",
                             argv [0]),
                            -1);
          break;
        }
    }

  if (producer_ior == 0)
    {
      producer_ior = "file://Producer.ior";
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, 
		                                        argv,
					                                  ""
					                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        {
          return -1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (producer_ior
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      EC_Benchmark::Producer_var producer
        = EC_Benchmark::Producer::_narrow (obj.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (producer.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR, 
                             "Unable to acquire 'Producer' objref\n"),
                            -1);
        }

      for (int i = 0; i < niterations; ++i)
        {
          producer->start (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Who is the culprit \n");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Uncaught CORBA exception\n"),
                        1);
    }
  ACE_ENDTRY;

  return 0;
}
