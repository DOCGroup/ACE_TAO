//
// $Id$
//
#include "ace/streams.h"

#include "orbsvcs/Naming/CosNaming_i.h"
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
    response_addr_((u_short) 0),
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


