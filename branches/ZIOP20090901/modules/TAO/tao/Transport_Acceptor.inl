// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::ULong
TAO_Acceptor::tag (void) const
{
  return this->tag_;
}

ACE_INLINE void
TAO_Acceptor::set_error_retry_delay (time_t delay)
{
  this->error_retry_delay_ = delay;
}

TAO_END_VERSIONED_NAMESPACE_DECL
