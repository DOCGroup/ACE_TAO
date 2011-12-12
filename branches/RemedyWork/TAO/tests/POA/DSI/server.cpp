
//=============================================================================
/**
 *  @file      server.cpp
 *
 *  $Id$
 *
 *    A server program for the File IDL module
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "Database_i.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"

static const ACE_TCHAR *ior_output_file = ACE_TEXT ("ior");

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior_output_file = get_opts.opt_arg ();
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
  FILE *output_file = ACE_OS::fopen (ior_output_file, "w");

  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IOR: %s\n",
                       ior_output_file),
                      -1);

  int result = 0;

  result = ACE_OS::fprintf (output_file,
                            "%s",
                            first_ior);

  ACE_OS::fclose (output_file);

  if (result < 0 ||
      static_cast<size_t> (result) != ACE_OS::strlen (first_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       first_ior,
                       ior_output_file),
                      -1);

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Narrow the object reference to a POA reference
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies (5);
      policies.length (5);

      // ID Assignment Policy
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      // Lifespan Policy
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      // Request Processing Policy
      policies[2] =
        root_poa->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT);

      // Servant Retention Policy
      policies[3] =
        root_poa->create_servant_retention_policy (PortableServer::RETAIN);

      // Id Uniqueness Policy
      policies[4] =
        root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID);


      ACE_CString name = "firstPOA";
      PortableServer::POA_var first_poa = root_poa->create_POA (name.c_str (),
                                                                poa_manager.in (),
                                                                policies);

      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }

      // Create a Database Agent Implementation object in first_poa
      DatabaseImpl::Agent database_agent_impl (orb.in (),
                                               first_poa.in ());

      PortableServer::ObjectId_var database_agent_oid =
        PortableServer::string_to_ObjectId ("DatabaseAgent");

      first_poa->activate_object_with_id (database_agent_oid.in (),
                                          &database_agent_impl);

      CORBA::Object_var database_agent =
        first_poa->id_to_reference (database_agent_oid.in ());

      // Get the IOR for the "DatabaseAgent" object
      CORBA::String_var database_agent_ior =
        orb->object_to_string (database_agent.in ());

      ACE_DEBUG ((LM_DEBUG,"%C\n",
                  database_agent_ior.in ()));

      int write_result = write_iors_to_file (database_agent_ior.in ());
      if (write_result != 0)
        return write_result;

      // set the state of the poa_manager to active i.e ready to process requests
      poa_manager->activate ();

      // Run the ORB
      orb->run ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
