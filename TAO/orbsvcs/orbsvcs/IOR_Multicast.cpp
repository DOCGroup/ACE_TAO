// $Id$

#include "IOR_Multicast.h"

ACE_HANDLE
TAO_IOR_Multicast::get_handle (void) const
{
  return this->mcast_dgram_.get_handle ();
}

TAO_IOR_Multicast::TAO_IOR_Multicast (char * ior,
                              u_short port,
                              const char *mcast_addr,
                              TAO_Service_ID service_id)
  : service_id_ (service_id),
    mcast_addr_ (port, mcast_addr),
    ior_ (ior),
    response_addr_ ((u_short) 0),
    response_ (response_addr_)
{
  // Use ACE_SOCK_Dgram_Mcast factory to subscribe to multicast group.
  if (this->mcast_dgram_.subscribe (this->mcast_addr_) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "subscribe"));
}

// destructor

TAO_IOR_Multicast::~TAO_IOR_Multicast (void)
{
  this->mcast_dgram_.unsubscribe ();
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
  struct
  {
    u_short reply_port;
    CORBA::Short service_id;
  } mcast_info;

  ssize_t retcode =
    this->mcast_dgram_.recv (&mcast_info,
                             sizeof (mcast_info),
                             this->remote_addr_);

  if (retcode == -1)
    return -1;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Received multicast.\n"));

  // @@ validate data string received is from a valid client here
  // @@ Probably not needed

  if (retcode != sizeof (mcast_info))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Reply to multicast not sent. Received %d bytes, expected %d.",
                       retcode,
                       sizeof (mcast_info)),
                      -1);

  // Confirm that we were meant to respond to this request.
  mcast_info.service_id = ntohs (mcast_info.service_id);
  if (mcast_info.service_id == this->service_id_)
    {
      // Convert port number received to network byte order and set port
      // number to reply;
      mcast_info.reply_port = ntohs (mcast_info.reply_port);
      this->remote_addr_.set_port_number (mcast_info.reply_port);

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
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Multicast was not for us."));
    }

  return 0;
}
