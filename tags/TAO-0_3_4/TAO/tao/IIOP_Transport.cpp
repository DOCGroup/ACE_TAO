// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//
// = DESCRIPTION
//
// = AUTHOR
//
// ============================================================================

#include "tao/IIOP_Transport.h"
#include "tao/GIOP.h"
#include "tao/Connect.h"

TAO_IIOP_Transport::TAO_IIOP_Transport (TAO_IIOP_Handler_Base* handler)
  : handler_(handler),
    tag_(TAO_IOP_TAG_INTERNET_IOP)
{
}

TAO_IIOP_Transport::~TAO_IIOP_Transport ()
{
}

TAO_IIOP_Server_Transport::TAO_IIOP_Server_Transport (TAO_Server_Connection_Handler *handler)
  :  TAO_IIOP_Transport(handler),
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

TAO_IIOP_Server_Transport::~TAO_IIOP_Server_Transport ()
{
}

TAO_IIOP_Client_Transport::~TAO_IIOP_Client_Transport ()
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
TAO_IIOP_Transport::resume_conn (ACE_Reactor *reactor)
{
  int result = reactor->resume_handler (this->handler_);
  // @@ Are these needed!!
  ACE_UNUSED_ARG (result);
  ACE_ASSERT (result == 0);
}

void
TAO_IIOP_Transport::close_conn ()
{
  this->handler_->handle_close ();
}

ACE_HANDLE
TAO_IIOP_Transport::handle(void)
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

// @@ TODO: this is a good candidate for an ACE routine, even more,
// all the code to write a Message_Block chain could be encapsulated
// in ACE.
ssize_t
TAO_IIOP_Transport::do_sendv (const iovec *iov, int iovcnt, int total_bytes)
{
  ssize_t writelen = 0;
  ACE_HANDLE h = this->handler_->get_handle ();

  // @@ would using writev instead of readv affect performance
  //    on WIN32?? fredk
  ssize_t n = ACE_OS::sendv (h, iov, iovcnt);

  if (n == -1 || n == total_bytes)
  {
    // either an error occured or we sent all the bytes!
    // in either case we are done.
    return n;
  }

  // for some reason we didn't send all the data.  Keep trying.
  // should be the exception ... we also copy the iovec to a temp
  // var so it can be manipulated without violating the const'ness
  // of iov.
  iovec *tmp_iov = ACE_const_cast(iovec *, iov);
  writelen = n;
  int s = 0;
  size_t offset = n;

  // iovcnt > 0, iov{0] ... iov[iovcnt-1]
  while ( writelen < total_bytes )
  {
    // if n < iov_len then not all the bytes were sent from the
    // current iov buf.  NOTE, we do a ACE_static_cast so that we can
    // have n = -1.  That is, iov_len is an unsigned long but n is a
    // signed long.
    while (s < iovcnt  &&
           offset >= ACE_static_cast (size_t, tmp_iov[s].iov_len))
    {
      offset -= tmp_iov[s].iov_len;
      s++;
    } // while

    char* base = ACE_reinterpret_cast (char*, tmp_iov[s].iov_base);
    size_t len = tmp_iov[s].iov_len;

    tmp_iov[s].iov_base = base + offset;
    tmp_iov[s].iov_len  = len - offset;
    n = ACE_OS::sendv (h, (const iovec *)(iov + s), iovcnt - s);
    tmp_iov[s].iov_base = base;
    tmp_iov[s].iov_len = len;

    if ( n == -1 )
      return n;

    offset += n;
    writelen += n;

  } // outer while (s < iovcnt), that is send *all* data!
  return writelen;
}

ssize_t
TAO_IIOP_Transport::send (const ACE_Message_Block *mblk)
{
//  if ( this->handler_ == 0)
//    return -1;
  // For the most part this was copied from GIOP::send_request
  // and friends.
  const int TAO_WRITEV_MAX = 16;
  iovec iov[TAO_WRITEV_MAX];
  int iovcnt = 0, totlen=0;
  ssize_t n=0, nbytes=0;
  for (const ACE_Message_Block *i = mblk; i ; i = i->cont ()) {
    // Make sure there is something to send!
    if (i->length () > 0) {
      iov[iovcnt].iov_base = i->rd_ptr ();
      iov[iovcnt].iov_len  = i->length ();
      totlen += i->length ();
      iovcnt++;


      // The buffer is full make a OS call.
      // @@ TODO this should be optimized on a per-platform basis, for
      // instance, some platforms do not implement writev() there we
      // should copy the data into a buffer and call send_n(). In
      // other cases there may be some limits on the size of the
      // iovec, there we should set TAO_WRITEV_MAX to that limit.
      if (iovcnt == TAO_WRITEV_MAX)
      {
        if ( (n = do_sendv ((const iovec *) iov, iovcnt, totlen)) < 1)
          return n;

        nbytes += n;
        iovcnt = totlen = 0;

      } // iovcnt == TAO_WRITEV_MAX
    } // i->length () > 0
  } // for ( over message blocks, in groups of TAO_WRITEV_MAX )

  // check for remaining buffers to be sent!
  if (iovcnt != 0)
  {
    if ( (n = do_sendv ((const iovec *)iov, iovcnt, totlen)) < 0 )
      return 0;

    nbytes += n;

  } // iovcnt != 0

  return nbytes;  // success
}

ssize_t
TAO_IIOP_Transport::send (const u_char *buf, size_t len)
{
  // @@ could have used handler_->peer()->send_n()
  return ACE::send_n (this->handler_->get_handle(), buf, len);
}

ssize_t
TAO_IIOP_Transport::send (const iovec *iov, int iovcnt)
{
  int totlen = 0;
  for (int i = 0; i <= iovcnt  ; i++)
    totlen += iov[i].iov_len;
  return do_sendv (iov, iovcnt, totlen);
}

ssize_t
TAO_IIOP_Transport::receive(char *buf, size_t len)
{
  return ACE::recv_n (this->handler_->get_handle (), buf, len);
}

ssize_t
TAO_IIOP_Transport::receive(char *buf, size_t len, int flags)
{
  return ACE::recv_n (this->handler_->get_handle (), buf, len, flags);
}

ssize_t
TAO_IIOP_Transport::receive(iovec *iov, int iovcnt)
{
  return ACE_OS::readv(this->handler_->get_handle (), iov, iovcnt);
}
