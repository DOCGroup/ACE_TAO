// -*- C++ -*-
// $Id$

ACE_INLINE CORBA_ServerRequest_ptr
CORBA_ServerRequest::_duplicate (CORBA_ServerRequest_ptr x)
{
  if (x != 0)
    {
      x->_incr_refcnt ();
    }

  return x;
}

ACE_INLINE CORBA_ServerRequest_ptr
CORBA_ServerRequest::_nil (void)
{
  return (CORBA_ServerRequest_ptr)0;
}

ACE_INLINE CORBA::Context_ptr
CORBA_ServerRequest::ctx (void) const
{
  return this->ctx_;
}

ACE_INLINE void
CORBA_ServerRequest::ctx (CORBA::Context_ptr ctx)
{
  this->ctx_ = ctx;
}

ACE_INLINE const char *
CORBA_ServerRequest::operation (void) const
{
  return this->orb_server_request_.operation ();
}

ACE_INLINE void
CORBA_ServerRequest::_tao_lazy_evaluation (int lazy_evaluation)
{
  this->lazy_evaluation_ = lazy_evaluation;
}

