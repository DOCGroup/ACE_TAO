// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION 
//    Test server for the AVStreams demo
//   
// = AUTHORS
//   Sumedh Mungee <sumedh@cs.wustl.edu>
//
// ============================================================================

#include "orbsvcs/AVStreams_i.h"

int
main (int argc, char ** argv)
{
  TAO_TRY
    {
      // Initialize the ORB
      CORBA::ORB_ptr orb = CORBA::ORB_init (argc, 
                                                argv, 
                                                0, 
                                                TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Initialize the Object Adapter
      CORBA::Object_var poa_object = 
	orb->resolve_initial_references ("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
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
      
      // create a streamctrl object
      TAO_Basic_StreamCtrl basic_stream_ctrl;
      
      PortableServer::ObjectId_var id = 
        PortableServer::string_to_ObjectId ("Basic_StreamCtrl");
      good_poa->activate_object_with_id (id.in (),
                                         &basic_stream_ctrl,
                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.
  
      CORBA::Object_var obj = 
        good_poa->id_to_reference (id.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      CORBA::String_var str =
        orb->object_to_string (obj.in (),
                               TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      ACE_DEBUG ((LM_DEBUG,
                  "The IOR is: <%s>\n", str.in ()));
      
      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Handle requests for this object until we're killed, or one of
      // the methods asks us to exit.
      if (orb->run () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);
      
      root_poa->destroy (CORBA::B_TRUE, 
			 CORBA::B_TRUE, 
			 TAO_TRY_ENV);
      TAO_CHECK_ENV;
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
  ACE_DEBUG ((LM_DEBUG, "Server is terminating"));
  return 0;
}

