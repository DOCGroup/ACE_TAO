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
#include "cubit_i.h"

// Global Variables
static int num_of_objs = 1;

// Parses the command line arguments and returns an error status

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "dn:");
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
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n] <num of cubit objects>"
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
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Initialize the Object Adapter
      CORBA::Object_var poa_object = 
	orb->resolve_initial_references("RootPOA");
      if (poa_object == 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize the POA.\n"),
			  1);

      PortableServer::POA_var root_poa =
	PortableServer::POA::_narrow (poa_object, TAO_TRY_ENV);
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
      PortableServer::POA_var good_poa =
	root_poa->create_POA ("RootPOA_is_BAD",
			      poa_manager.in (),
			      policies,
			      TAO_TRY_ENV);  
      TAO_CHECK_ENV;

      // Parse remaining command line and verify parameters.
      parse_args (argc, argv);

      // create a factory implementation
      Cubit_Factory_i factory_impl;

      PortableServer::ObjectId_var id = 
	PortableServer::string_to_ObjectId ("factory");
      good_poa->activate_object_with_id (id.in (),
					 &factory_impl,
					 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (TAO_debug_level > 0)
	{
	  CORBA::Object_var obj = 
	    good_poa->id_to_reference (id.in (), TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  CORBA::String_var str =
	    orb->object_to_string (obj.in (),
				   TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  ACE_DEBUG ((LM_DEBUG,
		      "The IOR is: <%s>\n", str.in ()));
	}

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Handle requests for this object until we're killed, or one of
      // the methods asks us to exit.
      if (orb->run () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);

      root_poa->destroy (CORBA::B_TRUE, 
			 CORBA::B_TRUE, 
			 TAO_TRY_ENV);
      TAO_CHECK_ENV
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
