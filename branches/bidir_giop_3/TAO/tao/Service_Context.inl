// -*- C++ -*-
//
// $Id$

ACE_INLINE
TAO_Service_Context::TAO_Service_Context (void)
  : service_context_ ()
{
}

ACE_INLINE IOP::ServiceContextList &
TAO_Service_Context::service_info (void)
{
  return this->service_context_;
}

ACE_INLINE const IOP::ServiceContextList &
TAO_Service_Context::service_info (void) const
{
  return this->service_context_;
}


ACE_INLINE void
TAO_Service_Context::set_context (IOP::ServiceId id,
                                  TAO_OutputCDR &cdr)
{
  this->set_context_i (id,
                       cdr);
}

ACE_INLINE void
TAO_Service_Context::set_context (IOP::ServiceContext &context,
                                  TAO_OutputCDR &cdr)
{
  this->set_context_i (context,
                       cdr);
}
