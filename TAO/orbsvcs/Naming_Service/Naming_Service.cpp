//
// $Id$
//
#include "ace/streams.h"

#include "CosNaming_i.h"
#include "Naming_Service.h"

// This is a startup for the naming server.
// This is used for testing of the Naming Service.

ACE_HANDLE
IOR_Multicast::get_handle (void) const
{
  return this->mcast_dgram_.get_handle ();
}

IOR_Multicast::IOR_Multicast (char * ior,
			      u_short port, 
			      const char *mcast_addr)
  : mcast_addr_ (port, mcast_addr),
    ior_ (ior),
    response_ (response_addr_)
{
  // Use ACE_SOCK_Dgram_Mcast factory to subscribe to multicast group.
  if (this->mcast_dgram_.subscribe (this->mcast_addr_) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "subscribe"));
}

// destructor 

IOR_Multicast::~IOR_Multicast (void)
{
  this->mcast_dgram_.unsubscribe ();    
}

int
IOR_Multicast::handle_timeout (const ACE_Time_Value &,
                               const void *)
{
  return 0;
}

int
IOR_Multicast::handle_input (ACE_HANDLE)
{
  ssize_t retcode = 
    this->mcast_dgram_.recv (this->buf_, 
			     BUFSIZ, 
			     this->remote_addr_);
  
  if (retcode == -1)
    return -1;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Received multicast.\n"));
  
  // @@ validate data string received is from a valid client here
  // @@ Probably not needed

  if (retcode != sizeof (CORBA::Short))
    {
      ACE_ERROR_RETURN ((LM_ERROR, 
			 "Reply to multicast not sent. Received %d bytes, expected %d.", 
			 retcode, 
			 sizeof(CORBA::Short)), -1);
    }

  // convert port number received to network byte order.
  CORBA::Short reply_port_number = ntohs (*(CORBA::Short *)this->buf_);

  // set port number to reply.
  this->remote_addr_.set_port_number (reply_port_number);
  
  // send the object reference for the naming service
  retcode = response_.send (this->ior_, 
			    ACE_OS::strlen (this->ior_) + 1, 
			    this->remote_addr_, 
			    0);

  ACE_DEBUG ((LM_DEBUG, 
	      "(%P|%t) ior_: <%s>\n"
	      " sent through port %u.\n"
	      "retcode=%d\n", 
	      this->ior_, 
	      this->remote_addr_.get_port_number (), 
	      retcode));

  if (retcode == -1)
    return -1;  

  return 0;
}

int
main (int argc, char ** argv)
{
  TAO_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, 
					    argv, 
					    "internet", 
					    TAO_TRY_ENV);
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
      // get reactor instance from TAO
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
