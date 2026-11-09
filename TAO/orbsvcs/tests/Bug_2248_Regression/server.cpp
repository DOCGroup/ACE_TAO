#include "Server_ORBInitializer.h"
#include "Server_Request_Interceptor.h"
#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"
#include "tao/ORBInitializer_Registry.h"

#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {};
  ACE_TCHAR const *merged_iorstr {};
  ACE_TCHAR const *key {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:k:f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case 'k':
        options.key = get_opts.opt_arg ();
        break;
      case 'f':
        options.merged_iorstr = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-k <key> "
                           "-f merged_iorstr"
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
  try
    {
      Server_Options options;
      Server_ORBInitializer *temp_initializer {};
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // We do the command line parsing first
      if (parse_args (argc, argv, options) != 0)
        return -1;
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies;
      policies.length (3);
      policies[0] = root_poa->create_id_assignment_policy (
          PortableServer::USER_ID);
      policies[1] = root_poa->create_implicit_activation_policy (
          PortableServer::NO_IMPLICIT_ACTIVATION);
      policies[2] = root_poa->create_lifespan_policy (
          PortableServer::PERSISTENT);

      PortableServer::POA_var poa = root_poa->create_POA (
          "PERS_POA", poa_manager.in (), policies);

      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          policies[i]->destroy ();
        }

      Simple_Server_i display_impl (orb.in (), ACE_TEXT_ALWAYS_CHAR (options.key));
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("IOGR_OID");

      poa->activate_object_with_id (id.in (), &display_impl);

      CORBA::Object_var server =
          poa->id_to_reference (id.in ());

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      if (options.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, decltype (&ACE_OS::fclose)> output_file (
            ACE_OS::fopen (options.ior_output_file, "w"), ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                               options.ior_output_file),
                              -1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      // Set the forward references in the server request interceptor.
      PortableInterceptor::ServerRequestInterceptor_var
        server_interceptor = temp_initializer->server_interceptor ();

      Simple_ServerRequestInterceptor_var interceptor =
        Simple_ServerRequestInterceptor::_narrow (
           server_interceptor.in ());

      if (CORBA::is_nil (interceptor.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Could not obtain reference to "
                           "server request interceptor.\n"),
                          -1);

      interceptor->forward_reference (ACE_TEXT_ALWAYS_CHAR (options.merged_iorstr));

      poa_manager->activate ();

      orb->run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }
  return 0;
}
