// $Id$

//===================================================================
//  = LIBRARY
//      TAO/tests/POA/Default_Servant/server
//
//  = FILENAME
//      server.cpp
//
//  = DESCRIPTION
//      A server program for the File IDL module
//
//  = AUTHOR
//     Irfan Pyarali
//
//====================================================================

#include "Database_i.h"
#include "ace/Get_Opt.h"

ACE_RCSID(DSI, server, "$Id$")

static char *ior_output_file = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior_output_file = get_opts.optarg;
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

  // Indicates successful parsing of command line.
  return 0;
}

static int
write_iors_to_file (const char *first_ior)
{
  if (ior_output_file == 0)
    // No filename was specified; simply return
    return 0;

  FILE *output_file = ACE_OS::fopen (ior_output_file, "w");

  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IOR: %s\n",
                       ior_output_file),
                      -1);

  int result = 0;

  result = ACE_OS::fprintf (output_file,
                            "%s",
                            first_ior);
  if (result < 0 ||
      ACE_static_cast (size_t, result) != ACE_OS::strlen (first_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       first_ior,
                       ior_output_file),
                      -1);

  ACE_OS::fclose (output_file);

  return 0;
}

int
main (int argc, char **argv)
{
  CORBA::Environment env;

  // Initialize the ORB
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  int result = parse_args (argc, argv);
  if (result != 0)
    return result;

  // Get the Root POA object reference
  CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

  // Narrow the object reference to a POA reference
  PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  PortableServer::POAManager_var poa_manager = root_poa->the_POAManager (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::the_POAManager");
      return -1;
    }

  CORBA::PolicyList policies (5);
  policies.length (5);

  // ID Assignment Policy
  policies[0] =
    root_poa->create_id_assignment_policy (PortableServer::USER_ID, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_id_assignment_policy");
      return -1;
    }

  // Lifespan Policy
  policies[1] =
    root_poa->create_lifespan_policy (PortableServer::PERSISTENT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_lifespan_policy");
      return -1;
    }

  // Request Processing Policy
  policies[2] =
    root_poa->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_request_processing_policy");
      return -1;
    }

  // Servant Retention Policy
  policies[3] =
    root_poa->create_servant_retention_policy (PortableServer::RETAIN, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_servant_retention_policy");
      return -1;
    }

  // Id Uniqueness Policy
  policies[4] =
    root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_id_uniqueness_policy");
      return -1;
    }

  ACE_CString name = "firstPOA";
  PortableServer::POA_var first_poa = root_poa->create_POA (name.c_str (),
                                                            poa_manager.in (),
                                                            policies,
                                                            env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  for (CORBA::ULong i = 0;
       i < policies.length () && env.exception () == 0;
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (env);
    }
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  // Create a Database Agent Implementation object in first_poa
  DatabaseImpl::Agent database_agent_impl (orb.in (),
                                           first_poa.in ());

  PortableServer::ObjectId_var database_agent_oid =
    PortableServer::string_to_ObjectId ("DatabaseAgent");

  first_poa->activate_object_with_id (database_agent_oid.in (),
                                      &database_agent_impl,
                                      env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::activate_object_with_id");
      return -1;
    }

  CORBA::Object_var database_agent =
    first_poa->id_to_reference (database_agent_oid.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::id_to_reference");
      return -1;
    }
  // Get the IOR for the "DatabaseAgent" object
  CORBA::String_var database_agent_ior =
    orb->object_to_string (database_agent.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,"%s\n",
              database_agent_ior.in ()));

  int write_result = write_iors_to_file (database_agent_ior.in ());
  if (write_result != 0)
    return write_result;

  // set the state of the poa_manager to active i.e ready to process requests
  poa_manager->activate (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::activate");
      return -1;
    }

  // Run the ORB
  if (orb->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

  // Destroy the rootPOA and its children
  root_poa->destroy (1,
                     1,
                     env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::destroy");
      return -1;
    }

  return 0;
}
