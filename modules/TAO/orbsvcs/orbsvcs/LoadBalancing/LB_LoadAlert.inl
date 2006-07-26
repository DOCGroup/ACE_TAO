// -*- C++ -*-
//
// $Id$


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Boolean
TAO_LB_LoadAlert::alerted (void) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, monitor, this->lock_, 0);

  return this->alerted_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
