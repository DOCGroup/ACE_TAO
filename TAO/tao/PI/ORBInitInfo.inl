// -*- C++ -*-

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_ORB_Core *
TAO_ORBInitInfo::orb_core () const
{
  return this->orb_core_;
}

ACE_INLINE void
TAO_ORBInitInfo::invalidate ()
{
  this->orb_core_ = 0;
}

ACE_INLINE PortableInterceptor::SlotId
TAO_ORBInitInfo::slot_count () const
{
  return this->slot_count_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
