#include "ace/Get_Opt.h"
#include "ping_i.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

struct Ping_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("ping.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Ping_Options &options)
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Ping_Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // create child poa with PERSISTENT policy
      CORBA::PolicyList policies {};
      policies.length (2);
      policies[0] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      policies[1] =
        root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

      PortableServer::POA_var persistent_poa =
        root_poa->create_POA ("persistent",
                             poa_manager.in (),
                             policies);
      policies[0]->destroy ();
      policies[1]->destroy ();

      poa_manager->activate ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      PingObject_i server_impl (orb.in (), persistent_poa.in ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&server_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      PingObject_var const server =
        PingObject::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (!options.ior_output_file.empty ())
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")),
            ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ACE_TEXT_ALWAYS_CHAR (options.ior_output_file.c_str ())),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "ping - event loop finished\n"));

      persistent_poa->destroy (true, true);

      root_poa->destroy (true, true);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in ping:");
      return 1;
    }

  return 0;
}
