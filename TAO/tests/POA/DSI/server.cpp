//=============================================================================
/**
 *  @file      server.cpp
 *
 *    A server program for the File IDL module
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#include "Database_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include <cstring>
#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT ("ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("f:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-f ior_output_file] "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

static int
write_iors_to_file (ACE_TCHAR const *ior_output_file, char const *ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                       ior_output_file),
                      -1);

  int const result = ACE_OS::fprintf (output_file.get (), "%s", ior);
  if (result < 0 || static_cast<size_t> (result) != std::strlen (ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       ior,
                       ior_output_file),
                      -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      Server_Options options {};
      int const result = parse_args (argc, argv, options);
      if (result != 0)
        return result;

      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      CORBA::PolicyList policies (5);
      policies.length (5);
      policies[0] = root_poa->create_id_assignment_policy (PortableServer::USER_ID);
      policies[1] = root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
      policies[2] = root_poa->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT);
      policies[3] = root_poa->create_servant_retention_policy (PortableServer::RETAIN);
      policies[4] = root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID);

      char const * const name = "firstPOA";
      PortableServer::POA_var first_poa =
        root_poa->create_POA (name,
                              poa_manager.in (),
                              policies);

      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr const policy = policies[i];
          policy->destroy ();
        }

      DatabaseImpl::Agent database_agent_impl (orb.in (), first_poa.in ());
      PortableServer::ObjectId_var database_agent_oid =
        PortableServer::string_to_ObjectId ("DatabaseAgent");
      first_poa->activate_object_with_id (database_agent_oid.in (), &database_agent_impl);

      CORBA::Object_var database_agent = first_poa->id_to_reference (database_agent_oid.in ());
      CORBA::String_var database_agent_ior = orb->object_to_string (database_agent.in ());
      ACE_DEBUG ((LM_DEBUG, "%C\n", database_agent_ior.in ()));

      int const write_result = write_iors_to_file (options.ior_output_file,
                                                   database_agent_ior.in ());
      if (write_result != 0)
        return write_result;

      poa_manager->activate ();
      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
