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
  return this->opname_;
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
