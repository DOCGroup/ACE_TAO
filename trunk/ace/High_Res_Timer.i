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

  // <scale_factor> takes precendence over the global_scale_factor_.
  if (scale_factor_ == 0)
    scale_factor_ = ACE_High_Res_Timer::global_scale_factor_;

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
    usecs = (ACE_hrtime_t) ((this->end_ - this->start_) / scale_factor_) / 1000;
  } else {
    usecs = (this->end_ - this->start_) / 1000;
  }
}

ACE_INLINE void
ACE_High_Res_Timer::global_scale_factor (double gsf)
{
  global_scale_factor_ = gsf;
}

ACE_INLINE void
ACE_High_Res_Timer::hrtime_to_tv (ACE_Time_Value &tv,
				  ACE_hrtime_t hrt, 
				  double scale_factor)
{
  if (scale_factor > 0) {
    tv.sec ((long) (hrt / scale_factor / 1000) / 1000000);
    tv.usec ((long) (hrt / scale_factor / 1000) % 1000000);
  } else {
    tv.sec ((long) (hrt / 1000) / 1000000);
    tv.usec ((long) (hrt / 1000) % 1000000);
  }
}

ACE_INLINE ACE_Time_Value
ACE_High_Res_Timer::gettimeofday (void)
{
  ACE_Time_Value tv;
  ACE_High_Res_Timer::hrtime_to_tv (tv,
				    ACE_OS::gethrtime (),
				    ACE_High_Res_Timer::global_scale_factor_);
  return tv;
}

