// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/IIOP_Transport.h"
#include "tao/GIOP.h"
#include "tao/Connect.h"

TAO_IIOP_Transport::TAO_IIOP_Transport (TAO_IIOP_Handler_Base* handler)
  : handler_(handler),
    tag_(TAO_IOP_TAG_INTERNET_IOP)
{
}

TAO_IIOP_Transport::~TAO_IIOP_Transport (void)
{
}

TAO_IIOP_Server_Transport::TAO_IIOP_Server_Transport (TAO_Server_Connection_Handler *handler)
  : TAO_IIOP_Transport(handler),
    server_handler_ (0)
{
  server_handler_ = handler;
}

TAO_IIOP_Client_Transport::TAO_IIOP_Client_Transport (TAO_Client_Connection_Handler *handler)
  :  TAO_IIOP_Transport(handler),
     client_handler_ (0)
{
  client_handler_ = handler;
}

TAO_IIOP_Server_Transport::~TAO_IIOP_Server_Transport (void)
{
}

TAO_IIOP_Client_Transport::~TAO_IIOP_Client_Transport (void)
{
}

CORBA::ULong
TAO_IIOP_Transport::tag (void)
{
  return this->tag_;
}

TAO_Client_Connection_Handler *
TAO_IIOP_Client_Transport::client_handler (void)
{
  return this->client_handler_;
}

TAO_Server_Connection_Handler *
TAO_IIOP_Server_Transport::server_handler (void)
{
  return this->server_handler_;
}

TAO_IIOP_Handler_Base *&
TAO_IIOP_Transport::handler (void)
{
  return this->handler_;
}

int
TAO_IIOP_Transport::idle (void)
{
  return this->handler_->idle();
}

int
TAO_IIOP_Transport::is_nil (TAO_Transport *obj)
{
  return obj == 0;
}

TAO_Transport *
TAO_IIOP_Transport::_nil (void)
{
  return (TAO_IIOP_Transport *)0;
}

void
TAO_IIOP_Transport::resume_connection (ACE_Reactor *reactor)
{
  int result = reactor->resume_handler (this->handler_);
  // @@ Are these needed!!
  ACE_UNUSED_ARG (result);
  ACE_ASSERT (result == 0);
}

void
TAO_IIOP_Transport::close_connection (void)
{
  this->handler_->handle_close ();
}

ACE_HANDLE
TAO_IIOP_Transport::handle (void)
{
  return this->handler_->get_handle ();
}

int
TAO_IIOP_Client_Transport::send_request (TAO_ORB_Core *orb_core,
                                         TAO_OutputCDR &stream,
                                         int twoway)
{
  return this->client_handler_->send_request (orb_core, stream, twoway);
}

// int
// TAO_IIOP_Server_Transport::send_response (TAO_OutputCDR &response)
// {
//   this->server_handler_->send_response (response);
//   return 1;
// }

ssize_t
TAO_IIOP_Transport::send (const ACE_Message_Block *mblk, ACE_Time_Value *s)
{
  ACE_UNUSED_ARG (s);

  // For the most part this was copied from GIOP::send_request and
  // friends.

  // @@ Fred, this should NOT be a local constant...  It should use a
  // macro defined in OS.h...
  const int TAO_WRITEV_MAX = 16;
  iovec iov[TAO_WRITEV_MAX];
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
          iov[iovcnt].iov_len  = i->length ();
          iovcnt++;

          // The buffer is full make a OS call.  @@ TODO this should
          // be optimized on a per-platform basis, for instance, some
          // platforms do not implement writev() there we should copy
          // the data into a buffer and call send_n(). In other cases
          // there may be some limits on the size of the iovec, there
          // we should set TAO_WRITEV_MAX to that limit.
          if (iovcnt == TAO_WRITEV_MAX)
            {
              n = this->handler_->peer ().sendv_n ((const iovec *) iov,
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
      n = this->handler_->peer ().sendv_n ((const iovec *) iov,
                                           iovcnt);
      if (n < 0 )
        return 0;

      nbytes += n;
    }

  return nbytes;
}

ssize_t
TAO_IIOP_Transport::send (const u_char *buf,
                          size_t len,
                          ACE_Time_Value *s)
{
  ACE_UNUSED_ARG (s);
  return this->handler_->peer ().send_n (buf, len);
}

ssize_t
TAO_IIOP_Transport::send (const iovec *iov,
                          int iovcnt,
                          ACE_Time_Value *s)
{
  ACE_UNUSED_ARG (s);
  return this->handler_->peer ().sendv_n ((const iovec *) iov,
                                          iovcnt);
}

ssize_t
TAO_IIOP_Transport::recv (char *buf,
                          size_t len,
                          ACE_Time_Value *s)
{
  ACE_UNUSED_ARG (s);
  return this->handler_->peer ().recv_n (buf, len);
}

ssize_t
TAO_IIOP_Transport::recv (char *buf,
                          size_t len,
                          int flags,
                          ACE_Time_Value *s)
{
  ACE_UNUSED_ARG (s);
  return this->handler_->peer ().recv_n (buf,
                                         len,
                                         flags);
}

ssize_t
TAO_IIOP_Transport::recv (iovec *iov,
                          int iovcnt,
                          ACE_Time_Value *s)
{
  ACE_UNUSED_ARG (s);
  return handler_->peer ().recvv_n (iov, iovcnt);
}
