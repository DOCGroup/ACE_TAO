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

ACE_INLINE TAO_InputCDR &
IIOP_ServerRequest::incoming (void)
{
  return *this->incoming_;
}

ACE_INLINE TAO_OutputCDR &
IIOP_ServerRequest::outgoing (void)
{
  return *this->outgoing_;
}

// Invocation attributes.

ACE_INLINE const char *
IIOP_ServerRequest::operation (void) const
{
  return this->operation_.c_str ();
}

// get the length of the operation name.
ACE_INLINE unsigned int
IIOP_ServerRequest::operation_length (void) const
{
  return this->operation_.length ();
}

#if 0
//@@ (TAO) to do after Context is implemented
ACE_INLINE CORBA::Context_ptr
IIOP_ServerRequest::ctx (void)
{
  return 0;
}
#endif

ACE_INLINE CORBA::Boolean
IIOP_ServerRequest::response_expected (void) const
{
  return this->response_expected_;
}

ACE_INLINE CORBA::Principal_ptr
IIOP_ServerRequest::principal (void) const
{
  return this->requesting_principal_;
}

ACE_INLINE const TAO_ObjectKey &
IIOP_ServerRequest::object_key (void) const
{
  return this->object_key_;
}

ACE_INLINE const TAO_GIOP_ServiceContextList &
IIOP_ServerRequest::service_info (void) const
{
  return this->service_info_;
}

ACE_INLINE CORBA::ULong
IIOP_ServerRequest::request_id (void)
{
  return this->request_id_;
}
