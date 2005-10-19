// -*- C++ -*-
//
// $Id$


ACE_INLINE CORBA::Boolean
TAO_LB_LoadAlert::alerted (void) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, monitor, this->lock_, 0);

  return this->alerted_;
}
