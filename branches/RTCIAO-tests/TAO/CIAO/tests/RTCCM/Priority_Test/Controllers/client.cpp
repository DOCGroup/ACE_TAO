// $Id$
/**
 * @file client.cpp
 *
 * This program interact with a Controller component
 */

#include "ControllerC.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"

char *controller_ior_ = 0;
long work = 80;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:w:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {

      case 'k':
        controller_ior_ = get_opts.opt_arg ();
        break;

      case 'w':
        work = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-k <Controller IOR>\n"
                           "-w <work amount>\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (controller_ior_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please provide controller IOR\n"),
                      -1);

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
        = orb->string_to_object (controller_ior_
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Priority_Test::Controller_var ctrlr
        = Priority_Test::Controller::_narrow (obj.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (ctrlr.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire 'Controller' objref\n"), -1);

      ctrlr->start (work
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
