// $Id$

#include "Progress_i.h"
#include "ace/Get_Opt.h"

ACE_RCSID(FL_Callback, progress, "$Id$")

#if !defined (ACE_HAS_FL)

int main (int, char *[])
{
  ACE_ERROR ((LM_INFO,
              "This example requires FL support\n"));
  return 0;
}

#else

#include <FL/Fl.h>
#include <FL/Fl_Window.h>

const char *ior_output_file = "progress.ior";
int n_iterations = 1000;
int n_peers = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:p:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'p':
        n_peers = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        n_iterations = ACE_OS::atoi (get_opts.opt_arg ());
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
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      int w = 320;
      int h = 30 * n_peers + 20;
      Fl_Window window(w, h);

      Progress_Window sw (n_peers, n_iterations,
                          10,
                          10,
                          window.w () - 20,
                          window.h () - 20);
      window.resizable (&sw);

      window.end ();

      char* targv[] = { argv[0] };
      window.show (1, targv);

      sw.show ();

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
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

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Progress_i server_impl (&sw);

      Progress_var server =
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

      if (Fl::run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Fl::run"), -1);
      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
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

#endif /* ACE_HAS_FL */
