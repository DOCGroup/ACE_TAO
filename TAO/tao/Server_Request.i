// -*- C++ -*-
// $Id$

// @@ TODO TAO allocates its ServerRequest objects from the stack,
// using reference counting wouldn't help much.  Using a _clone()
// method would make the following methods really easy to implement,
// but that's hard to implement for the extremely optimized
// IIOP_ServerRequest.
// Another solution would be to modify the class hierarchy, make
// ServerRequest the "slow" version (with copies for each field) and
// IIOP_ServerRequest the "fast" version (with no copies at all). The
// first would be use for DII and the later for SII.

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA_ServerRequest *x)
{
  return x != 0;
}

ACE_INLINE void
CORBA::release (CORBA_ServerRequest *)
{
}

ACE_INLINE CORBA_ServerRequest *
CORBA_ServerRequest::_duplicate (CORBA_ServerRequest *)
{
  return 0;
}

ACE_INLINE CORBA_ServerRequest *
CORBA_ServerRequest::_nil (void)
{
  return 0;
}

#if (TAO_HAS_MINIMUM_CORBA == 0)

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

#endif /* TAO_HAS_MINIMUM_CORBA */

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

