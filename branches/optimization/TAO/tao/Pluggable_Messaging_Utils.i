// -*- C++ -*-

//$Id$



ACE_INLINE void
TAO_Pluggable_Reply_Params::service_context_notowned (IOP::ServiceContextList *svc)
{
  this->service_context_ = svc;
}

ACE_INLINE IOP::ServiceContextList &
TAO_Pluggable_Reply_Params::service_context_notowned (void)
{
  return *this->service_context_;
}
