ACE_INLINE CDR &
IIOP_ServerRequest::incoming (void)
{
  return *this->incoming_;
}

ACE_INLINE CDR &
IIOP_ServerRequest::outgoing (void)
{
  return *this->outgoing_;
}

ACE_INLINE void 
IIOP_ServerRequest::release (void) 
{ 
  refcount_--; 
}
