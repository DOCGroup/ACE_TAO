// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::ServerRequest_ptr
CORBA::ServerRequest::_duplicate (CORBA::ServerRequest_ptr x)
{
  if (x != 0)
    {
      x->_incr_refcnt ();
    }

  return x;
}

ACE_INLINE CORBA::ServerRequest_ptr
CORBA::ServerRequest::_nil (void)
{
  return (CORBA::ServerRequest_ptr)0;
}

ACE_INLINE CORBA::Context_ptr
CORBA::ServerRequest::ctx (void) const
{
  return this->ctx_;
}

ACE_INLINE void
CORBA::ServerRequest::ctx (CORBA::Context_ptr ctx)
{
  this->ctx_ = ctx;
}

ACE_INLINE const char *
CORBA::ServerRequest::operation (void) const
{
  return this->orb_server_request_.operation ();
}

ACE_INLINE void
CORBA::ServerRequest::_tao_lazy_evaluation (int lazy_evaluation)
{
  this->lazy_evaluation_ = lazy_evaluation;
}

ACE_INLINE int
CORBA::ServerRequest::_tao_incoming_byte_order (void) const
{
  return this->orb_server_request_.incoming ().byte_order ();
}

ACE_INLINE void
CORBA::ServerRequest::_tao_reply_byte_order (int byte_order)
{
  this->orb_server_request_.outgoing ().reset_byte_order (byte_order);
}


ACE_INLINE TAO_ServerRequest &
CORBA::ServerRequest::_tao_server_request (void)
{
  return this->orb_server_request_;
}
