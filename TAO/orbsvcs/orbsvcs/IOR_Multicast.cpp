// $Id$

#include "IOR_Multicast.h"

ACE_RCSID(orbsvcs, IOR_Multicast, "$Id$")

ACE_HANDLE
TAO_IOR_Multicast::get_handle (void) const
{
  return this->mcast_dgram_.get_handle ();
}

TAO_IOR_Multicast::TAO_IOR_Multicast (void)
  : service_id_ ((TAO_Service_ID) 0),
    ior_ (0)
{
}

TAO_IOR_Multicast::TAO_IOR_Multicast (const char *ior,
                                      u_short port,
                                      const char *mcast_addr,
                                      TAO_Service_ID service_id)
{
  if (this->init (ior,
                  port,
                  mcast_addr,
                  service_id) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("TAO_IOR_Multicast")));
}

// destructor

TAO_IOR_Multicast::~TAO_IOR_Multicast (void)
{
  this->mcast_dgram_.unsubscribe ();
}

int
TAO_IOR_Multicast::init (const char *ior,
                         u_short port,
                         const char *mcast_addr,
                         TAO_Service_ID service_id)
{
  this->service_id_ = service_id;
  this->mcast_addr_.set (port, mcast_addr);
  this->ior_ = ior;
  this->response_addr_.set ((u_short) 0);
  this->response_.open (this->response_addr_);

  // Use ACE_SOCK_Dgram_Mcast factory to subscribe to multicast group.
  if (this->mcast_dgram_.subscribe (this->mcast_addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "subscribe"),
                      -1);
  return 0;
}

int
TAO_IOR_Multicast::handle_timeout (const ACE_Time_Value &,
                                   const void *)
{
  return 0;
}

int
TAO_IOR_Multicast::handle_input (ACE_HANDLE)
{
  // mcast_info[0] == port.
  // mcast_info[1] == service id.
  ACE_UINT16 mcast_info[2];

  ACE_INET_Addr remote_addr;

  ssize_t result =
    // This call fills in the address of the sender.
    this->mcast_dgram_.recv (mcast_info,
                             sizeof (mcast_info),
                             remote_addr);

  if (result == -1)
    return -1;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) Received multicast.\n"));

  // Validate data string received is from a valid client here.

  if (result != sizeof (mcast_info))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Reply to multicast not sent. Received %d bytes, expected %d.",
                       result,
                       sizeof (mcast_info)),
                      -1);

  // Confirm that we were meant to respond to this request.
  mcast_info[1] = ACE_NTOHS (mcast_info[1]);

  if (mcast_info[1] == this->service_id_)
    {
      remote_addr.set_port_number (mcast_info[0], 0);

      // Send the object reference for the naming service

      // Wait to give client a chance to start receiving,
      // Send two times to provide better protection against packet
      // loss.
      // @@Vishal, a better (not hardcoded) 'policy' solution for this should be provided.
      for (int i = 0; i < 5; ++i)
        {
          ACE_OS::sleep (ACE_Time_Value (0, 10000));
          result = response_.send (this->ior_,
                                   ACE_OS::strlen (this->ior_),
                                   remote_addr,
                                   0);
        }

      if (TAO_debug_level > 0)
	ACE_DEBUG ((LM_DEBUG,
		    "(%P|%t) ior_: <%s>\n"
		    " sent through port %u to host %s.\n"
		    "result = %d\n",
		    this->ior_,
		    remote_addr.get_port_number (),
		    remote_addr.get_host_name (),
		    result));

      if (result == -1)
        return -1;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) Multicast was not for us."));

  return 0;
}
