// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/IDL_Cubit
//
// = FILENAME
//    svr.cpp
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee and Sergio Flores-Gaitan
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"
#include "cubit_i.h"

// Global Variables
static int num_of_objs = 1;

//Flag to tell whether to use the Naming service
static int use_naming_service;

// Parses the command line arguments and returns an error status
static FILE* ior_output_file = 0;

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "dn:o:s");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'n':	// number of cubit objects we hold
        num_of_objs = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'o':
        ior_output_file = ACE_OS::fopen (get_opts.optarg, "w");
        if (ior_output_file == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
      case 's':
	use_naming_service=1;
	break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n] <num of cubit objects>"
			   " [-o] <ior_output_file>"
                           "\n", argv [0]), 1);
      }

  return 0;  // Indicates successful parsing of command line
}

// Standard command line parsing utilities used.

int
main (int argc, char *argv[])
{
  TAO_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, 0, TAO_TRY_ENV);

      TAO_CHECK_ENV;

      // Initialize the Object Adapter
      CORBA::Object_var poa_object = 
	orb->resolve_initial_references ("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize the POA.\n"),
			  1);

      PortableServer::POA_var root_poa =
	PortableServer::POA::_narrow (poa_object.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::POAManager_var poa_manager =
	root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;
  
      PortableServer::PolicyList policies (2);
      policies.length (2);  
      policies[0] =
	root_poa->create_id_assignment_policy (PortableServer::USER_ID,
					       TAO_TRY_ENV);
      policies[1] =
	root_poa->create_lifespan_policy (PortableServer::PERSISTENT,
					  TAO_TRY_ENV);

      // We use a different POA, otherwise the user would have to
      // change the object key each time it invokes the server.
      PortableServer::POA_var poa =
	root_poa->create_POA ("RootPOA_is_BAD",
			      poa_manager.in (),
			      policies,
			      TAO_TRY_ENV);  
      TAO_CHECK_ENV;

      //Parse remaining command line and verify parameters.
      parse_args (argc, argv);

      // create a factory implementation
      Cubit_Factory_i factory_impl;

      PortableServer::ObjectId_var id = 
	PortableServer::string_to_ObjectId ("factory");
      poa->activate_object_with_id (id.in (),
					 &factory_impl,
					 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::Object_var obj = 
	poa->id_to_reference (id.in (),
                              TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var str =
	orb->object_to_string (obj.in (),
			       TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG,
		  "The IOR is: <%s>\n", str.in ()));
      if (ior_output_file)
        {
          ACE_OS::fprintf (ior_output_file, "%s", str.in());
          ACE_OS::fclose (ior_output_file);
        }

      if (use_naming_service) 
	{
	  //Resolve the Naming Service context
	  CORBA::Object_var naming_obj =
	    orb->resolve_initial_references ("NameService");
	  if (CORBA::is_nil (naming_obj.in ()))
	    ACE_ERROR_RETURN ((LM_ERROR,
			       " (%P|%t) Unable to resolve the Name Service.\n"),
			      2);
	  
	  CosNaming::NamingContext_var naming_context = 
	    CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  // Bind a new IDL_Cubit Name context
	  CosNaming::Name cubit_context_name (1);
	  cubit_context_name.length (1);
	  cubit_context_name[0].id = CORBA::string_dup ("IDL_Cubit");
	  CosNaming::NamingContext_var cubit_context =
	    naming_context->bind_new_context (cubit_context_name,TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  
	  //Register the cubit_factory name with the IDL_Cubit Naming Context...
	  CosNaming::Name factory_name (1);
	  factory_name.length (1);
	  factory_name[0].id = CORBA::string_dup ("cubit_factory");
	  cubit_context->bind (factory_name,obj.in (), TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	}

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Handle requests for this object until we're killed, or one of
      // the methods asks us to exit.
      if (orb->run () == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "run"),
                          -1);
      root_poa->destroy (CORBA::B_TRUE, 
			 CORBA::B_TRUE, 
			 TAO_TRY_ENV);
      TAO_CHECK_ENV
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG(sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG(userex);
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
