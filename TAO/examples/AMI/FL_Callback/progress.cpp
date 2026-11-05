#include "Progress_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "tao/FlResource/FlResource_Loader.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include <cstdio>
#include <memory>

struct Progress_Options
{
  ACE_TCHAR const *ior_output_file { ACE_TEXT ("progress.ior") };
  int n_iterations {1000};
  int n_peers {1};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Progress_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:p:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case 'p':
        options.n_peers = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        options.n_iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-p <peers>"
                           "-i <iterations>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO::FlResource_Loader fl_loader;
  try
    {
      Progress_Options options;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      int const w = 320;
      int const h = 30 * options.n_peers + 20;
      Fl_Window window(w, h);

      Progress_Window sw (options.n_peers, options.n_iterations,
                          10,
                          10,
                          window.w () - 20,
                          window.h () - 20);
      window.resizable (&sw);

      window.end ();

      char* targv[] = { ACE_TEXT_ALWAYS_CHAR (argv[0]) };
      window.show (1, targv);

      sw.show ();

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

      poa_manager->activate ();

      Progress_i server_impl (&sw);

      Progress_var server =
        server_impl._this ();

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (options.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file, "w"), ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               options.ior_output_file),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      if (Fl::run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Fl::run"), -1);
      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  return 0;
}
