// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/bin/Logger
//
// = FILENAME
//    ior_multicast.cpp
//
// = DESCRIPTION 
//    Listens to multicast address for client requests for ior's.
//    
// = AUTHORS
//      Sergio Flores-Gaitan
//
// ============================================================================

#include "ior_multicast.h"

ACE_HANDLE
IOR_Multicast::get_handle (void) const
{
  return this->mcast_dgram_.get_handle ();
}

IOR_Multicast::IOR_Multicast (char * ior,
			    u_short port, 
			    const char *mcast_addr,
			    u_short response_port)
  : ior_ (ior),
    mcast_addr_ (port, mcast_addr),
    SERVICE_RESPONSE_UDP_PORT_ (response_port),
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
                               const void *arg)
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

  ACE_DEBUG ((LM_ERROR, "@@ Received multicast @@\n"));
  
  // @@ validate data string received is from a valid client here
  // @@ Probably not needed

  this->remote_addr_.set_port_number (this->SERVICE_RESPONSE_UDP_PORT_);
  retcode = response_.send (this->ior_, 
			    ACE_OS::strlen (this->ior_) + 1, 
			    this->remote_addr_, 
			    0);

  ACE_DEBUG ((LM_ERROR, "ior_ '%s' sent.\nretcode=%d\n", this->ior_, retcode));

  if (retcode == -1)
    return -1;  

  return 0;
}
