/* -*- C++ -*- */
// $Id$

// High_Res_Timer.i

ACE_INLINE int
ACE_High_Res_Timer::supported ()
{
  return ACE_OS::gethrtime () != -1;
}

ACE_INLINE
ACE_High_Res_Timer::ACE_High_Res_Timer (double scale_factor)
  : scale_factor_ (scale_factor)
{
  ACE_TRACE ("ACE_High_Res_Timer::ACE_High_Res_Timer");
  this->reset ();
}

ACE_INLINE void
ACE_High_Res_Timer::start (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::start");
  this->start_ = ACE_OS::gethrtime ();
}

ACE_INLINE void
ACE_High_Res_Timer::stop (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::stop");
  this->end_ = ACE_OS::gethrtime ();
}

ACE_INLINE void
ACE_High_Res_Timer::start_incr (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::start_incr");
  this->start_incr_ = ACE_OS::gethrtime ();
}

ACE_INLINE void
ACE_High_Res_Timer::stop_incr (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::stop_incr");
  this->total_ += ACE_OS::gethrtime () - this->start_incr_;
}

ACE_INLINE void
ACE_High_Res_Timer::elapsed_microseconds (ACE_hrtime_t &usecs) const
{
  if (scale_factor_ > 0) {
    usecs = (this->end_ - this->start_) / 1000L / scale_factor_;
  } else {
    usecs = (this->end_ - this->start_) / 1000L;
  }
}
