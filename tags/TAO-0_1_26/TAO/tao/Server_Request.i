// $Id$

ACE_INLINE void
CORBA::release (CORBA_ServerRequest *req)
{
   if (req)
     req->Release ();
}

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
  return this->operation_;
}

#if 0
//@@ (TAO) to do after Context is implemented
ACE_INLINE CORBA::Context_ptr
IIOP_ServerRequest::ctx (void)
{
  return 0;
}
#endif

ACE_INLINE CORBA::ORB_ptr
IIOP_ServerRequest::orb (void)
{
  return orb_;
}

ACE_INLINE TAO_POA *
IIOP_ServerRequest::oa (void)
{
  return poa_;
}

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

ACE_INLINE const TAO_opaque &
IIOP_ServerRequest::object_key (void) const
{
  return this->object_key_;
}

ACE_INLINE const TAO_GIOP_ServiceContextList &
IIOP_ServerRequest::service_info (void) const
{
  return this->service_info_;
}
