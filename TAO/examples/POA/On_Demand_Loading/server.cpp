// $Id$

//============================================================================
//
//  =FILENAME
//     server.cpp
//
//  =DESCRIPTION
//     Server to test the Servant Activator. The servant activator
//     loads a servant on demand and makes its association with the
//     object.
//
//  =AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//=============================================================================

#include "ace/streams.h"
#include "Servant_Activator.h"

ACE_RCSID(On_Demand_Activation, server, "$Id$")

static char *ior_output_file = 0;

// The input arguments are accepted.

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

// The IOR is written to a file.

static int
write_iors_to_file (const char *first_ior)
{
  if (ior_output_file == 0)
    // No filename was specified; simply return
    return 0;

  char ior_output_file_1[BUFSIZ];


  ACE_OS::sprintf (ior_output_file_1, "%s", ior_output_file);


  FILE *output_file_1 = ACE_OS::fopen (ior_output_file_1, "w");

  
  if (output_file_1 == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IORs: %s, %s\n", 
                       ior_output_file_1),
                      -1);

  int result = ACE_OS::fprintf (output_file_1,
				"%s", 
				first_ior);
  if (result <= 0
      || ACE_static_cast(size_t,result) != ACE_OS::strlen (first_ior))
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "ACE_OS::fprintf failed while writing %s to %s\n", 
                       first_ior,
                       ior_output_file_1),
                      -1);
  
 
  
  ACE_OS::fclose (output_file_1);
  
  return 0;
}

int
main (int argc, char **argv)
{
  CORBA::ORB_var orb (0);
  PortableServer::POA_var root_poa;
  PortableServer::POAManager_var poa_manager;
  TAO_TRY_EX (1)
    {

      // Initialize the ORB.
      orb = CORBA::ORB_init (argc, argv, 0, TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (1);

    }
  TAO_CATCHANY 
    {
      TAO_TRY_ENV.print_exception ("CORBA::ORB_init");
      return 1;
    }
  TAO_ENDTRY;

  TAO_TRY_EX (2)
    {

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;
  
      // Get an Object reference to RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Narrow the Object reference to a POA reference
      root_poa =
        PortableServer::POA::_narrow (obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (2);
    }
  TAO_CATCHANY
    {
     
      TAO_TRY_ENV.print_exception ("PortableServer::POA::_narrow");
      return 1;
    }
  TAO_ENDTRY;

  // Get the POAManager of RootPOA
  TAO_TRY_EX (3)
    {
      poa_manager =
        root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (3);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("PortableServer::POA::the_POAManager");
      return 1;
    }
  TAO_ENDTRY;

  CORBA::PolicyList policies (4);
  policies.length (4);

  TAO_TRY_EX (4)
    {
      // ID Assignment Policy
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID, TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (4);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("PortableServer::POA::create_id_assignment_policy");
      return 1;
    }
  TAO_ENDTRY;

  // Lifespan Policy
  TAO_TRY_EX (5)
    {
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT, TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (5);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("PortableServer::POA::create_lifespan_policy");
      return 1;
    }
  TAO_ENDTRY;

  // Request Processing Policy
  TAO_TRY_EX (6)
    {
      policies[2] =
        root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER, TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (6);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("PortableServer::POA::create_request_processing_policy");
      return 1;
    }
  TAO_ENDTRY;
 
  PortableServer::POA_var first_poa;
  TAO_TRY_EX (7)
    {
      // Servant Retention Policy
      policies[3] =
        root_poa->create_servant_retention_policy (PortableServer::RETAIN, TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (7);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("PortableServer::POA::create_servant_retention_policy");
      return 1;
    }
  TAO_ENDTRY;
  
  TAO_TRY_EX (8)
    {
      ACE_CString name = "firstPOA";
    
      // Create firstPOA as the child of RootPOA with the above policies
      // firstPOA will use SERVANT_ACTIVATOR because of RETAIN policy.
      first_poa = root_poa->create_POA (name.c_str (),
                                        poa_manager.in (),
                                        policies,
                                        TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (8);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("PortableServer::POA::create_POA");
      return 1;
    }
  TAO_ENDTRY;

  TAO_TRY_EX (9)
    {
    
      // Destroy the policy objects as they have been passed to create_POA
      // and no longer needed.
      for (CORBA::ULong i = 0;
           i < policies.length () && TAO_TRY_ENV.exception () == 0;
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (TAO_TRY_ENV);
          TAO_CHECK_ENV_EX (9);
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("PortableServer::POA::create_POA");
      return 1;
    }
  TAO_ENDTRY;

  Dir_Service_Activator servant_activator_impl (orb.in ());        
  PortableServer::ServantActivator_var servant_activator;
  TAO_TRY_EX (10)
    {
      servant_activator =
        servant_activator_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (10);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("PortableServer::POAManager::_this");
      return 1;
    }
  TAO_ENDTRY;

  TAO_TRY_EX (11)
    {
      // Set Dir_Service_Activator object as the servant_manager of
      // firstPOA.
      first_poa->set_servant_manager (servant_activator.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (11);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("PortableServer::POAManager::set_servant_manager");
      return 1;
    }
  TAO_ENDTRY;

  PortableServer::ObjectId_var first_dir_service_oid;
  CORBA::Object_var first_dir_service;
  TAO_TRY_EX (12)
    {
      // Create a reference with user created ID in firstPOA which uses
      // the Dir_Service_Activator. Note the format of the ObjectID specified.
      // Its library_name:factory_method which is then used by the servant activator to 
      // obtain and load the right servant.
      first_dir_service_oid =
        PortableServer::string_to_ObjectId ("DirService:create_dir_service");
  
      first_dir_service =
        first_poa->create_reference_with_id (first_dir_service_oid.in (), "IDL:Dir_Service:1.0", TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (12);
    }
  TAO_CATCHANY
    {
 
      TAO_TRY_ENV.print_exception ("PortableServer::POA::create_reference_with_id");
      return 1;
    }
  TAO_ENDTRY;

  CORBA::String_var first_dir_service_ior;
  TAO_TRY_EX (13)
    {
      // Invoke object_to_string on the references created in firstPOA
      first_dir_service_ior =
        orb->object_to_string (first_dir_service.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (13);
    }
  TAO_CATCHANY
    {
  
      TAO_TRY_ENV.print_exception ("CORBA::ORB::object_to_string");
      return 1;
    }
  TAO_ENDTRY;


  TAO_TRY_EX (14)
    {
 
      // Print the ior's of first_dir_service 
      ACE_DEBUG((LM_DEBUG,"%s\n",
                 first_dir_service_ior.in ()));

      int write_result = write_iors_to_file (first_dir_service_ior.in ());

      if (write_result != 0)
        return write_result;

      // Set the poa_manager state to active, ready to process requests.
      poa_manager->activate (TAO_TRY_ENV);
  
      TAO_CHECK_ENV_EX (14);
    }
  TAO_CATCHANY
    {
  
      TAO_TRY_ENV.print_exception ("PortableServer::POAManager::activate");
      return 1;
    }
  TAO_ENDTRY;

  TAO_TRY_EX (15)
    {
 
      // Run the ORB.
      if (orb->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

      // Destroy the root_poa and also first_poa and second_poa
      root_poa->destroy (1,
                         1,
                         TAO_TRY_ENV);
      TAO_CHECK_ENV_EX (15);
    }
  TAO_CATCHANY
    {

      TAO_TRY_ENV.print_exception ("PortableServer::POA::destroy");
      return 1;
    }

  TAO_ENDTRY;

  return 0;
}
