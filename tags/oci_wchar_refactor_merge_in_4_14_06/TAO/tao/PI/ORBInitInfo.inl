// -*- C++ -*-
//
// $Id$


ACE_INLINE TAO_ORB_Core *
TAO_ORBInitInfo::orb_core (void) const
{
  return this->orb_core_;
}

ACE_INLINE void
TAO_ORBInitInfo::invalidate (void)
{
  this->orb_core_ = 0;
}

ACE_INLINE PortableInterceptor::SlotId
TAO_ORBInitInfo::slot_count (void) const
{
  return this->slot_count_;
}
