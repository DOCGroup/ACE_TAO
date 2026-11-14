#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/XtResource/XtResource_Loader.h"
#include <cstdio>
#include <memory>
#include <Xm/Xm.h>
#include "Stopwatch_display.h"
#include "timer.h"

static int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                                     "-o <iorfile>"
                                     "\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior_output_file {};

  // We do the command line parsing first
  if (parse_args (argc, argv, ior_output_file) != 0)
    return 1;

  XtAppContext app {};
  Widget const toplevel = XtAppInitialize (&app,
                                           "Stopwatch",
                                           0,
                                           nullptr,
                                           &argc,
                                           argv,
                                           nullptr,
                                           nullptr,
                                           0);

  TAO::XtResource_Loader xt_loader (app);

  Stopwatch_display stopwatch (toplevel);
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      stopwatch.manage ();

      // Make a timer class
      Timer_imp timer (app, 100, &stopwatch);

      Stopwatch_imp server_impl (orb.in (), &timer);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&server_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Stopwatch_var const server =
        Stopwatch::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (ior_output_file, "w"), std::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                               ior_output_file),
                              1);
          std::fprintf (output_file.get (), "%s", ior.in ());
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
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  return 0;
}

