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


#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "File_i.h"

ACE_RCSID(Default_Servant, server, "$Id$")

static char *ior_output_file = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "o:d");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.optarg;
        break;
      case 'd':
	TAO_debug_level++;
	break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-oior_output_file]"
			   "[-d]"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int 
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  // Initialize the ORB
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  int result = parse_args (argc, argv);
  if (result != 0)
    return result;
 
  // Get the Root POA object reference
  CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

  // Narrow the object reference to a POA reference
  PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  PortableServer::POAManager_var poa_manager = root_poa->the_POAManager (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  CORBA::PolicyList policies (5);
  policies.length (5);  

  // ID Assignment Policy
  policies[0] =
    root_poa->create_id_assignment_policy (PortableServer::USER_ID, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Lifespan Policy
  policies[1] =
    root_poa->create_lifespan_policy (PortableServer::PERSISTENT, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Request Processing Policy
  policies[2] =
    root_poa->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Servant Retention Policy
  policies[3] =
    root_poa->create_servant_retention_policy (PortableServer::RETAIN, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  // Id Uniqueness Policy
  policies[4] =
    root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  ACE_CString name = "firstPOA";
  PortableServer::POA_var first_poa = root_poa->create_POA (name.c_str (),
                                                            poa_manager.in (),
                                                            policies,
                                                            ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  for (CORBA::ULong i = 0;
       i < policies.length () && ACE_TRY_ENV.exception () == 0;
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (ACE_TRY_ENV);
    }  
  ACE_CHECK_RETURN (-1);


  // Create a File System Implementation object in first_poa
  FileImpl::System file_system_impl (first_poa.in ());

  PortableServer::ObjectId_var file_system_oid =
    PortableServer::string_to_ObjectId ("FileSystem");

  first_poa->activate_object_with_id (file_system_oid.in (), 
                                      &file_system_impl, 
                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
    
  CORBA::Object_var file_system =
    first_poa->id_to_reference (file_system_oid.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Get the IOR for the "FileSystem" object
  CORBA::String_var file_system_ior =
    orb->object_to_string (file_system.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,"%s\n",
		file_system_ior.in ()));
  
  // If the ior_output_file exists, output the ior to it
  if (ior_output_file != 0)
    {
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
	ACE_ERROR_RETURN ((LM_DEBUG, "Cannot open output file for writing IOR: %s", 
			   ior_output_file),
			  -1);  
      ACE_OS::fprintf (output_file, "%s", file_system_ior.in ());
      ACE_OS::fclose (output_file);
    }
  
  // set the state of the poa_manager to active i.e ready to process requests
  poa_manager->activate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Run the ORB
  if (orb->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

  // Destroy the rootPOA and its children
  root_poa->destroy (1, 
                     1, 
                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}


