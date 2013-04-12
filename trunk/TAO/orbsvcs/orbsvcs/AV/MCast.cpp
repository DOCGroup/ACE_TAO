// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/MCast.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/AV/MCast.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//------------------------------------------------------------
//TAO_AV_UDP_MCast_Flow_Handler
//------------------------------------------------------------

TAO_AV_UDP_MCast_Flow_Handler::TAO_AV_UDP_MCast_Flow_Handler (void)
{
  ACE_NEW (transport_,
           TAO_AV_UDP_MCast_Transport (this));
  ACE_NEW (dgram_mcast_,
           ACE_SOCK_Dgram_Mcast);
}

TAO_AV_UDP_MCast_Flow_Handler::~TAO_AV_UDP_MCast_Flow_Handler (void)
{
  delete this->transport_;
  delete this->dgram_mcast_;
}


int
TAO_AV_UDP_MCast_Flow_Handler::handle_input (ACE_HANDLE /*fd*/)
{
  return this->protocol_object_->handle_input ();
//  return 0;
}

int
TAO_AV_UDP_MCast_Flow_Handler::handle_timeout (const ACE_Time_Value &tv,
                                               const void *arg)
{
  return TAO_AV_Flow_Handler::handle_timeout (tv,arg);
}

ACE_HANDLE
TAO_AV_UDP_MCast_Flow_Handler::get_handle (void) const
{
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_UDP_MCast_Flow_Handler::get_handle "));
  return this->get_mcast_socket ()->get_handle () ;
}

void
TAO_AV_UDP_MCast_Flow_Handler::set_peer_addr (ACE_INET_Addr *peer_addr)
{
  this->peer_addr_ = peer_addr;
}

//------------------------------------------------------------
// TAO_AV_UDP_MCast_Transport
//------------------------------------------------------------

TAO_AV_UDP_MCast_Transport::TAO_AV_UDP_MCast_Transport (void)
  :handler_ (0)
{
}

TAO_AV_UDP_MCast_Transport::TAO_AV_UDP_MCast_Transport (TAO_AV_UDP_MCast_Flow_Handler *handler)
  :handler_ (handler)
{
}

TAO_AV_UDP_MCast_Transport::~TAO_AV_UDP_MCast_Transport (void)
{
}

int
TAO_AV_UDP_MCast_Transport::open (ACE_Addr * /*address*/)
{
  return 0;
}

int
TAO_AV_UDP_MCast_Transport::close (void)
{
  return 0;
}


ACE_Addr*
TAO_AV_UDP_MCast_Transport::get_peer_addr (void)
{
  return &this->peer_addr_;
}

ACE_Addr*
TAO_AV_UDP_MCast_Transport::get_local_addr (void)
{
  this->handler_->get_mcast_socket ()->get_local_addr (this->local_addr_);
  return &this->local_addr_;
}

ssize_t
TAO_AV_UDP_MCast_Transport::send (const ACE_Message_Block *mblk, ACE_Time_Value *)
{
  // For the most part this was copied from GIOP::send_request and
  // friends.

  iovec iov[ACE_IOV_MAX];
  int iovcnt = 0;
  ssize_t n = 0;
  ssize_t nbytes = 0;

  for (const ACE_Message_Block *i = mblk;
       i != 0;
       i = i->cont ())
    {
      // Make sure there is something to send!
      if (i->length () > 0)
        {
          iov[iovcnt].iov_base = i->rd_ptr ();
          iov[iovcnt].iov_len  = static_cast<u_long> (i->length ());
          iovcnt++;

          // The buffer is full make a OS call.  @@ TODO this should
          // be optimized on a per-platform basis, for instance, some
          // platforms do not implement writev() there we should copy
          // the data into a buffer and call send_n(). In other cases
          // there may be some limits on the size of the iovec, there
          // we should set ACE_IOV_MAX to that limit.
          if (iovcnt == ACE_IOV_MAX)
            {
              n = this->handler_->get_mcast_socket ()->send ((const iovec *) iov,
                                                             iovcnt);

              if (n < 1)
                return n;

              nbytes += n;
              iovcnt = 0;
            }
        }
    }

  // Check for remaining buffers to be sent!
  if (iovcnt != 0)
    {
      n = this->handler_->get_mcast_socket ()->send ((const iovec *) iov,
                                                     iovcnt);
      if (n < 1)
        return n;

      nbytes += n;
    }

  return nbytes;
}

ssize_t
TAO_AV_UDP_MCast_Transport::send (const char *buf,
                                  size_t len,
                                  ACE_Time_Value *)
{
//   if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_UDP_MCast_Transport::send "));
//   char addr [BUFSIZ];
//   this->peer_addr_.addr_to_string (addr,BUFSIZ);
//   if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"to %s\n",addr));

  return this->handler_->get_mcast_socket ()->send (buf, len);

}

ssize_t
TAO_AV_UDP_MCast_Transport::send (const iovec *iov,
                                  int iovcnt,
                                  ACE_Time_Value *)
{
  return this->handler_->get_mcast_socket ()->send (iov,
                                                    iovcnt,
                                                    0);

}

int
TAO_AV_UDP_MCast_Transport::mtu (void)
{
  return ACE_MAX_DGRAM_SIZE;
}


ssize_t
TAO_AV_UDP_MCast_Transport::recv (char *buf,
                                  size_t len,
                                  ACE_Time_Value *)
{
  return this->handler_->get_mcast_socket ()->recv (buf, len,this->peer_addr_);
}

ssize_t
TAO_AV_UDP_MCast_Transport::recv (char *buf,
                                  size_t len,
                                  int flags,
                                  ACE_Time_Value *timeout)
{
  return this->handler_->get_mcast_socket ()->recv (buf,
                                                    len,
                                                    this->peer_addr_,
                                                    flags,
                                                    timeout);
}

ssize_t
TAO_AV_UDP_MCast_Transport::recv (iovec *iov,
                                  int /*iovcnt*/,
                            ACE_Time_Value *timeout)
{
  return handler_->get_mcast_socket ()->recv (iov,this->peer_addr_,0,timeout);
}

TAO_END_VERSIONED_NAMESPACE_DECL
