// -*- C++ -*-
// $Id$

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA_ServerRequest *x)
{
  return x != 0;
}

// @@ TODO TAO allocates its ServerRequest objects from the stack,
// using reference counting wouldn't help much.  Using a _clone()
// method would make the following methods really easy to implement,
// but that's hard to implement for the extremely optimized
// IIOP_ServerRequest.
// Another solution would be to modify the class hierarchy, make
// ServerRequest the "slow" version (with copies for each field) and
// IIOP_ServerRequest the "fast" version (with no copies at all). The
// first would be use for DII and the later for SII.

ACE_INLINE CORBA_ServerRequest *
CORBA_ServerRequest::_duplicate (CORBA_ServerRequest *)
{
  return 0;
}

ACE_INLINE void
CORBA::release (CORBA_ServerRequest *)
{
}

ACE_INLINE CORBA_ServerRequest *
CORBA_ServerRequest::_nil (void)
{
  return 0;
}
