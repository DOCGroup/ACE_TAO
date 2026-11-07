// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "test_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT ("test.ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Server_Options options {};
      if (parse_args (argc, argv, options) != 0)
        return 1;

      {
        Database_i server_impl (orb.in (),
                                128); // @@ TODO

        PortableServer::ObjectId_var id =
          root_poa->activate_object (&server_impl);

        CORBA::Object_var object = root_poa->id_to_reference (id.in ());

        Test::Database_var server =
          Test::Database::_narrow (object.in ());

        CORBA::String_var ior =
          orb->object_to_string (server.in ());

        ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

        // If the ior_output_file exists, output the ior to it
        if (options.ior_output_file != nullptr)
          {
            std::unique_ptr<FILE, int (*) (FILE *)> output_file (
              ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
            if (!output_file)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                                 options.ior_output_file),
                                1);
            ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
          }

        poa_manager->activate ();

        orb->run ();

        ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
        ACE_OS::sleep (1);
        root_poa->destroy (true, true);
      }
      ACE_OS::sleep (1);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
