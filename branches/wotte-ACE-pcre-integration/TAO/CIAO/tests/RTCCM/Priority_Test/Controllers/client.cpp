// $Id$
/**
 * @file client.cpp
 *
 * This program interact with a Controller component
 */

#include "ControllerC.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"

int ctrl_off = 0;
long work = 80;
typedef ACE_Unbounded_Stack<ACE_CString> STRING_STACK;
typedef ACE_Unbounded_Stack<Priority_Test::Controller_var> CTRL_STACK;

int
parse_args (int argc, char *argv[], STRING_STACK &iors)
{
  ACE_Get_Opt get_opts (argc, argv, "k:fw:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {

      case 'k':
        iors.push (get_opts.opt_arg ());
        break;

      case 'f':                 // Turn off controller
        ctrl_off = 1;
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
                           "-f: Turn off Controller\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (iors.is_empty () && ctrl_off == 0)
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
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, ""  ACE_ENV_ARG_PARAMETER);

      STRING_STACK IORs;

      if (parse_args (argc, argv, IORs) != 0)
        return -1;


      CTRL_STACK controllers;

      while (!IORs.is_empty ())
        {
          ACE_CString ior;
          IORs.top (ior);
          CORBA::Object_var obj
            = orb->string_to_object (ior.c_str ()
                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          Priority_Test::Controller_var ctrlr
            = Priority_Test::Controller::_narrow (obj.in ()
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (ctrlr.in ()))
            {
              ACE_DEBUG ((LM_ERROR,
                          "Unable to acquire 'Controller' objref\n",
                          ior.c_str ()));
            }
          else
            {
              controllers.push (ctrlr);
            }
          IORs.pop (ior);

        }

      while (! controllers.is_empty ())
        {
          Priority_Test::Controller_var ctrlr;
          controllers.top (ctrlr);

          if (ctrl_off != 0)
            ctrlr->stop (ACE_ENV_SINGLE_ARG_PARAMETER);
          else
            ctrlr->start (work
                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          controllers.pop (ctrlr);
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
