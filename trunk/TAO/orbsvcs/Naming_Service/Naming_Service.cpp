// $Id$

#include "ace/streams.h"
#include "orbsvcs/Naming/CosNaming_i.h"
#include "orbsvcs/Naming/Ior_Multicast.h"

int
main (int argc, char ** argv)
{
  TAO_TRY
    {
      // @@ Naga, can you please integrate the TAO_ORB_Manager
      // component here to further simplify main?
      CORBA::ORB_var orb = CORBA::ORB_init (argc, 
					    argv, 
					    "internet", 
					    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Initialize the Object Adapter
      CORBA::Object_var poa_object = 
	orb->resolve_initial_references("RootPOA");
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

      // We use a different POA. The RootPOA lifespan policy is
      // "TRANSIENT", hence whatever object key we export (for
      // instance in an environment variable) is invalid next time we
      // run.  To avoid this (and make the user life easier) we create
      // a new POA:
  
      PortableServer::PolicyList policies (2);
      policies.length (2);  
      policies[0] =
	root_poa->create_id_assignment_policy (PortableServer::USER_ID,
					       TAO_TRY_ENV);
      policies[1] =
	root_poa->create_lifespan_policy (PortableServer::PERSISTENT,
					  TAO_TRY_ENV);

      PortableServer::POA_var good_poa =
	root_poa->create_POA ("RootPOA_is_BAD",
			      poa_manager.in (),
			      policies,
			      TAO_TRY_ENV);  
      TAO_CHECK_ENV;

      // Create a naming context object.
      NS_NamingContext naming_context_impl;

      PortableServer::ObjectId_var id = 
	PortableServer::string_to_ObjectId ("NameService");
      good_poa->activate_object_with_id (id.in (),
					 &naming_context_impl,
					 TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.  Then release the object.
      CORBA::Object_var obj = 
	good_poa->id_to_reference (id.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var str =
	orb->object_to_string (obj.in (),
			       TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "listening as object <%s>\n", str.in ()));

#if defined (ACE_HAS_IP_MULTICAST)
      // @@ Naga, it looks like we're duplicating this code in all the
      // tests now.  Can you please see if you can figure a way to
      // factor it out so we can reuse it as a method in one of the
      // helper classes?

      // Get reactor instance from TAO.
      ACE_Reactor *reactor = TAO_ORB_Core_instance ()->reactor ();
  
      // First, see if the user has given us a multicast port number
      // for the name service on the command-line;
      u_short port = TAO_ORB_Core_instance ()->orb_params ()->name_service_port ();

      if (port == 0)
	{
	  const char *port_number = ACE_OS::getenv ("NameServicePort");

	  if (port_number != 0)
	    port = ACE_OS::atoi (port_number);
	}

      if (port == 0)
	port = TAO_DEFAULT_NAME_SERVER_REQUEST_PORT;

      // Instantiate a server which will receive requests for an ior
      IOR_Multicast ior_multicast (str,
				   port,
				   ACE_DEFAULT_MULTICAST_ADDR);

      // register event handler for the ior multicast.
      if (reactor->register_handler (&ior_multicast,
				     ACE_Event_Handler::READ_MASK) == -1)
	ACE_ERROR ((LM_ERROR, "%p\n%a", "register_handler", 1));
    
      ACE_DEBUG ((LM_DEBUG, "The multicast server setup is done.\n"));
#endif /* ACE_HAS_IP_MULTICAST */

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
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Naming Service");
      return 1;
    }
  TAO_ENDTRY;

  return 0;
}
