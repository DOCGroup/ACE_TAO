#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/SString.h"
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("server.ior")};
  int threads {4};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:n:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case 'n':
        options.threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Server_Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to initialize the POA.\n"),
                            1);
        }

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv, options) != 0)
        {
          return 1;
        }

      Simple_Server_i server_impl (orb.in ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&server_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      CORBA::String_var const ior = orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file <%s> for writing IOR\n"),
                             options.ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Server: exception caught - ");
      return 1;
    }

  return 0;
}
