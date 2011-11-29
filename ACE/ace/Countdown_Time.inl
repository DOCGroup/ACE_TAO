// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE bool
ACE_Countdown_Time::stopped (void) const
{
  return stopped_;
}

ACE_INLINE void
ACE_Countdown_Time::update (void)
{
  this->stop ();
  this->start ();
}

ACE_INLINE ACE_Time_Value
ACE_Countdown_Time::gettimeofday (void)
{
  if (this->gettimeofday_)
    return this->gettimeofday_ ();
  else
    return ACE_OS::gettimeofday ();
}

ACE_END_VERSIONED_NAMESPACE_DECL
