// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE RTCORBA::RTORB_ptr
TAO_Notify_RT_Properties::rt_orb (void)
{
  return RTCORBA::RTORB::_duplicate (rt_orb_.in ());
}

ACE_INLINE void
TAO_Notify_RT_Properties::rt_orb (RTCORBA::RTORB_ptr rt_orb)
{
  rt_orb_ = RTCORBA::RTORB::_duplicate (rt_orb);
}

ACE_INLINE RTCORBA::Current_ptr
TAO_Notify_RT_Properties::current (void)
{
  return RTCORBA::Current::_duplicate (current_.in());
}

ACE_INLINE void
TAO_Notify_RT_Properties::current (RTCORBA::Current_ptr current)
{
  current_ = RTCORBA::Current::_duplicate (current);
}

TAO_END_VERSIONED_NAMESPACE_DECL
