// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (Xt_Stopwatch,
           server,
           "$Id$")

#include "tao/XtResource_Loader.h"
#include <Xm/Xm.h>
#include "Stopwatch_display.h"
#include "timer.h"

const char *ior_output_file = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  // We do the command line parsing first
  if (parse_args (argc, argv) != 0)
    return 1;

  XtAppContext app;
  Widget toplevel = XtAppInitialize (&app,
                                     "Stopwatch",
                                     NULL,
                                     0,
                                     &argc,
                                     argv,
                                     NULL,
                                     NULL,
                                     0);

  TAO::XtResource_Loader xt_loader (app);

  Stopwatch_display stopwatch (toplevel);

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      stopwatch.manage ();

      // Make a timer class
      Timer_imp timer (app, 100, &stopwatch);

      Stopwatch_imp server_impl (orb.in (), &timer);

      Stopwatch_var server =
        server_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
      {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
      }

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      XtRealizeWidget (toplevel);
      /* Looks like there seems to be a problem with ST cases using
         XtAppMainLoop. Havent been able to figure out what the
         problem could be. The funny part is that orb->run () works
         fine. The XtRector actually calls XtAppProcessEvent (), which
         actually does something similar to this. Need to investigate
         this when we have time*/
#if defined (ACE_HAS_THREADS)
      XtAppMainLoop (app);
#else
      orb->run ();
#endif /*ACE_HAS_THREADS*/
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

