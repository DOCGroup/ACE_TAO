/* -*- C++ -*- */
// $Id$

// High_Res_Timer.i

ACE_INLINE void
ACE_High_Res_Timer::hrtime_to_tv (ACE_Time_Value &tv,
				  ACE_hrtime_t hrt)
{
  // The following are based on the units of global_scale_factor_
  // being 1/microsecond.  Therefore, dividing by it converts
  // clock ticks to microseconds.
  tv.sec ((long) (hrt / global_scale_factor_) / 1000000);
  tv.usec ((long) (hrt / global_scale_factor_) % 1000000);
}


ACE_INLINE ACE_Time_Value
ACE_High_Res_Timer::gettimeofday (void)
{
#if defined (ACE_WIN32)
  // If the global_scale_factor_ == 1 and we're on an Intel platform,
  // then a scale factor is needed by the platform gethrtime.  Since
  // one has not been set, just return ACE_OS::gettimeofday.
  if (ACE_High_Res_Timer::global_scale_factor_ == 1)
    return ACE_OS::gettimeofday ();
#endif /* ACE_WIN32 */

  ACE_Time_Value tv;
  ACE_High_Res_Timer::hrtime_to_tv (tv,
				    ACE_OS::gethrtime ());
  return tv;
}


ACE_INLINE ACE_hrtime_t
ACE_High_Res_Timer::gettime (void)
{
#if defined (ACE_WIN32)
  // If the global_scale_factor_ == 1 and we're on an Intel platform,
  // then a scale factor is needed by the platform gethrtime.  Since
  // one has not been set, just return ACE_OS::gettimeofday.
  if (ACE_High_Res_Timer::global_scale_factor_ == 1)
    {
      ACE_Time_Value tv = ACE_OS::gettimeofday ();
      // Return the time in microseconds because the global_scale_factor_
      // is 1.
      return tv.sec () * 1000000 + tv.usec ();
    }
#endif /* ACE_WIN32 */

  return ACE_OS::gethrtime ();
}

ACE_INLINE
ACE_High_Res_Timer::ACE_High_Res_Timer (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::ACE_High_Res_Timer");

  this->reset ();
}

ACE_INLINE void
ACE_High_Res_Timer::start (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::start");
  this->start_ = ACE_High_Res_Timer::gettime ();
}

ACE_INLINE void
ACE_High_Res_Timer::stop (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::stop");
  this->end_ = ACE_High_Res_Timer::gettime ();
}

ACE_INLINE void
ACE_High_Res_Timer::start_incr (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::start_incr");
  this->start_incr_ = ACE_High_Res_Timer::gettime ();
}

ACE_INLINE void
ACE_High_Res_Timer::stop_incr (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::stop_incr");
  this->total_ += ACE_High_Res_Timer::gettime () - this->start_incr_;
}

ACE_INLINE void
ACE_High_Res_Timer::elapsed_microseconds (ACE_hrtime_t &usecs) const
{
  usecs = (ACE_hrtime_t) ((this->end_ - this->start_) / global_scale_factor_);
}

ACE_INLINE void
ACE_High_Res_Timer::global_scale_factor (u_long gsf)
{
  global_scale_factor_ = gsf;
}
