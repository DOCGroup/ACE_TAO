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
  tv.sec ((long) (hrt / global_scale_factor_) / ACE_ONE_SECOND_IN_USECS);
  tv.usec ((long) (hrt / global_scale_factor_) % ACE_ONE_SECOND_IN_USECS);
}


ACE_INLINE ACE_Time_Value
ACE_High_Res_Timer::gettimeofday (const ACE_OS::ACE_HRTimer_Op op)
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
				    ACE_OS::gethrtime (op));
  return tv;
}


ACE_INLINE ACE_hrtime_t
ACE_High_Res_Timer::gettime (const ACE_OS::ACE_HRTimer_Op op)
{
#if defined (ACE_WIN32)
  // If the global_scale_factor_ == 1 and we're on an Intel platform,
  // then a scale factor is needed by the platform gethrtime.  Since
  // one has not been set, just return ACE_OS::gettimeofday.
  if (ACE_High_Res_Timer::global_scale_factor_ == 1)
    {
      ACE_Time_Value tv = ACE_OS::gettimeofday (op);
      // Return the time in microseconds because the global_scale_factor_
      // is 1.
      return tv.sec () * ACE_ONE_SECOND_IN_USECS + tv.usec ();
    }
#endif /* ACE_WIN32 */

  return ACE_OS::gethrtime (op);
}

ACE_INLINE
ACE_High_Res_Timer::ACE_High_Res_Timer (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::ACE_High_Res_Timer");

  this->reset ();
}

ACE_INLINE void
ACE_High_Res_Timer::start (const ACE_OS::ACE_HRTimer_Op op)
{
  ACE_TRACE ("ACE_High_Res_Timer::start");
  this->start_ = ACE_High_Res_Timer::gettime (ACE_OS::ACE_HRTIMER_START);
}

ACE_INLINE void
ACE_High_Res_Timer::stop (const ACE_OS::ACE_HRTimer_Op op)
{
  ACE_TRACE ("ACE_High_Res_Timer::stop");
  this->end_ = ACE_High_Res_Timer::gettime (ACE_OS::ACE_HRTIMER_STOP);
}

ACE_INLINE void
ACE_High_Res_Timer::start_incr (const ACE_OS::ACE_HRTimer_Op op)
{
  ACE_TRACE ("ACE_High_Res_Timer::start_incr");
  this->start_incr_ = ACE_High_Res_Timer::gettime (ACE_OS::ACE_HRTIMER_START);
}

ACE_INLINE void
ACE_High_Res_Timer::stop_incr (const ACE_OS::ACE_HRTimer_Op op)
{
  ACE_TRACE ("ACE_High_Res_Timer::stop_incr");
  this->total_ += ACE_High_Res_Timer::gettime (ACE_OS::ACE_HRTIMER_STOP) -
    this->start_incr_;
}

ACE_INLINE void
ACE_High_Res_Timer::elapsed_microseconds (ACE_hrtime_t &usecs) const
{
  usecs = (ACE_hrtime_t) ((this->end_ - this->start_) / global_scale_factor_);
}

ACE_INLINE void
ACE_High_Res_Timer::global_scale_factor (ACE_UINT32 gsf)
{
  global_scale_factor_ = gsf;
}

ACE_INLINE ACE_UINT32
ACE_High_Res_Timer::global_scale_factor ()
{
  return global_scale_factor_;
}
