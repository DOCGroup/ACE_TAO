#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include <cstdio>
#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {};
  ACE_TCHAR const *key {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          options.ior_output_file = get_opts.opt_arg ();
          break;
        case 'k':
          options.key = get_opts.opt_arg ();
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -o <iorfile> -k <key>\n"),
                             argv[0]),
                            -1);
        }
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      Server_Options options {};

      if (parse_args (argc, argv, options) != 0
          || options.ior_output_file == nullptr
          || options.key == nullptr)
        return 1;
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

      CORBA::PolicyList policies {};
      policies.length (3);
      policies[0] = root_poa->create_id_assignment_policy (
          PortableServer::USER_ID);
      policies[1] = root_poa->create_implicit_activation_policy (
          PortableServer::NO_IMPLICIT_ACTIVATION);
      policies[2] = root_poa->create_lifespan_policy (
          PortableServer::PERSISTENT);

      PortableServer::POA_var const poa = root_poa->create_POA (
          "PERS_POA", poa_manager.in (), policies);

      CORBA::ULong const policy_count = policies.length ();
      for (CORBA::ULong i {}; i < policy_count; ++i)
        {
          policies[i]->destroy ();
        }

      Simple_Server_i display_impl (orb.in (), ACE_TEXT_ALWAYS_CHAR (options.key));
      PortableServer::ObjectId_var const id =
          PortableServer::string_to_ObjectId ("IOGR_OID");

      poa->activate_object_with_id (id.in (), &display_impl);

      CORBA::Object_var const server =
          poa->id_to_reference (id.in ());

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")), std::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                             options.ior_output_file),
                            1);
        if (std::fprintf (output_file.get (), "%s", ior.in ()) < 0)
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot write IOR to output file\n")), 1);
      }

      poa_manager->activate ();

      orb->run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  return 0;
}
