// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/XtResource/XtResource_Loader.h"
#include <Xm/Xm.h>
#include "Stopwatch_display.h"
#include "timer.h"

const ACE_TCHAR *ior_output_file = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // We do the command line parsing first
  if (parse_args (argc, argv) != 0)
    return 1;

  XtAppContext app;
  Widget toplevel = XtAppInitialize (&app,
                                     "Stopwatch",
                                     0,
                                     0,
                                     &argc,
                                     argv,
                                     0,
                                     0,
                                     0);

  TAO::XtResource_Loader xt_loader (app);

  Stopwatch_display stopwatch (toplevel);


  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      stopwatch.manage ();

      // Make a timer class
      Timer_imp timer (app, 100, &stopwatch);

      Stopwatch_imp server_impl (orb.in (), &timer);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&server_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Stopwatch_var server =
        Stopwatch::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

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

      poa_manager->activate ();

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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  return 0;
}

