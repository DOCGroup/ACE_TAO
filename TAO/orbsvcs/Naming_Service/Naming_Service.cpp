//
// $Id$
//
#include <iostream.h>

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

  ACE_DEBUG ((LM_DEBUG, "Received multicast.\n"));
  
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
	      "ior_ '%s' sent through port %u.\nretcode=%d\n", 
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
  CORBA::Environment env;
  char *orb_name = "internet";

  CORBA::ORB_ptr orb_ptr = CORBA::ORB_init (argc, 
					    argv, 
					    orb_name, 
					    env);

  if (env.exception () != 0)
    {
      env.print_exception ("ORB init");
      return 1;
    }

  // Initialize the Object Adapter
  CORBA::POA_ptr oa_ptr = orb_ptr->POA_init (argc, argv, "POA");

  if (oa_ptr == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to initialize the POA.\n"),
		      1);

  // Create a naming context object.
  NS_NamingContext *naming_context = new NS_NamingContext ("NameService");
  
  // Stringify the objref we'll be implementing, and print it to
  // stdout.  Someone will take that string and give it to a
  // client.  Then release the object.
  CosNaming::NamingContext_ptr obj =
	  naming_context->_this (env);
  if (env.exception () != 0)
  {
	env.print_exception ("_this");
	return 1;
  }
  CORBA::String str;
  str = ACE_OS::strdup (orb_ptr->object_to_string (obj, env));

  if (env.exception () != 0)
    {
      env.print_exception ("object2string");
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG, "listening as object '%s'\n", str));
  CORBA::release (obj);

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

  // Handle requests for this object until we're killed, or one of the
  // methods asks us to exit.
  if (orb_ptr->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);

  // free memory with "free" because we used strdup which uses malloc
  ACE_OS::free (str);

  return 0;
}
