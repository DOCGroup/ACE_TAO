/* -*- C++ -*- */
// $Id$

// Be very carefull before changing the calculations inside
// ACE_High_Res_Timer.  The precision matters and we are using integer
// calculations not floating point.  Also look good at the emulated 64
// bit int class (inside Basic_Types{h,i,cpp} before changing
// anything.  It's operator/ only returns 32 bits not 64 bits, among
// other things.

ACE_INLINE void
ACE_High_Res_Timer::hrtime_to_tv (ACE_Time_Value &tv,
                                  const ACE_hrtime_t hrt)
{
  // The following are based on the units of global_scale_factor_
  // being 1/microsecond.  Therefore, dividing by it converts
  // clock ticks to microseconds.
  tv.sec ((long) (hrt / (ACE_UINT32) ACE_HR_SCALE_CONVERSION /
                  global_scale_factor ()));

  // Calculate usec in a manner that's compatible with ACE_U_LongLong.
  // hrt = (tv.sec * ACE_ONE_SECOND_IN_USECS + tv.usec) * global_scale_factor_
  // tv.usec = hrt / global_scale_factor_ - tv.sec * ACE_ONE_SECOND_IN_USECS
  // That first term will be lossy, so factor out global_scale_factor_:
  // tv.usec = (hrt - tv.sec * ACE_ONE_SECOND_IN_USECS * global_scale_factor_)/
  //           global_scale_factor
  ACE_hrtime_t tmp = tv.sec ();
  tmp *= ((ACE_UINT32) ACE_HR_SCALE_CONVERSION * global_scale_factor ());
#if defined (ACE_WIN32)
  // Win32's scale factor is in ticks/msec, so multiply up to usec.
  ACE_hrtime_t subsec = hrt - tmp;         // Remainder of ticks < 1sec
  ACE_UINT32 msec = (ACE_UINT32) (subsec / global_scale_factor ());  // #msec
  ACE_hrtime_t usec64 = subsec - (msec * global_scale_factor ());
#  if defined (ACE_LACKS_LONGLONG_T)
  ACE_UINT32 usec = usec64.lo();
#  else
  ACE_UINT32 usec = (ACE_UINT32) usec64;
#  endif // ACE_LACKS_LONGLONG_T
  //     (tick * usec/msec) / tick/msec     = usec
  usec = (usec * 1000)      / (ACE_UINT32) global_scale_factor ();
  tv.usec ((msec * 1000) + usec);
#else
  tv.usec ((long) ((hrt - tmp) / global_scale_factor ()));
#endif
}


ACE_INLINE ACE_Time_Value
ACE_High_Res_Timer::gettimeofday (const ACE_OS::ACE_HRTimer_Op op)
{
#if defined (ACE_WIN32)
  // Get the global scale factor if there isn't one yet.
  if (ACE_High_Res_Timer::global_scale_factor_status_ == 0)
    ACE_High_Res_Timer::global_scale_factor ();

  // If there isn't a high-res timer, use gettimeofday ();
  if (ACE_High_Res_Timer::global_scale_factor_status_ == -1)
    return ACE_OS::gettimeofday ();
#endif /* ACE_WIN32 */

  ACE_Time_Value tv;
  ACE_High_Res_Timer::hrtime_to_tv (tv,
                                    ACE_OS::gethrtime (op));
  return tv;
}


// Get the current high res timer as the time of day. This is intended
// to be used for a gettimeofday replacement in ACE_Timer_Queue and
// derived classes so the timers will bebased on high res timers rather
// than wall clock time. It uses the ACE_High_Res_Timer::gettimeofday
// function, which is deprecated. If it gets removed, please move the
// code down here, intact.
ACE_INLINE ACE_Time_Value
ACE_High_Res_Timer::gettimeofday_hr (void)
{
  return ACE_High_Res_Timer::gettimeofday ();
}


ACE_INLINE ACE_hrtime_t
ACE_High_Res_Timer::gettime (const ACE_OS::ACE_HRTimer_Op op)
{
#if defined (ACE_WIN32)
  // Get the global scale factor if there isn't one yet.
  if (ACE_High_Res_Timer::global_scale_factor_status_ == 0)
    ACE_High_Res_Timer::global_scale_factor ();

  // If there isn't a high-res timer, use gettimeofday ();
  if (ACE_High_Res_Timer::global_scale_factor_status_ == -1)
    {
      ACE_Time_Value tv = ACE_OS::gettimeofday ();
      // Return the time in microseconds because the global_scale_factor_
      // is 1.
      return tv.sec () * ACE_ONE_SECOND_IN_USECS + tv.usec ();
    }
#endif /* ACE_WIN32 */

  return ACE_OS::gethrtime (op);
}

ACE_INLINE
ACE_High_Res_Timer::~ACE_High_Res_Timer (void)
{
}

ACE_INLINE void
ACE_High_Res_Timer::start (const ACE_OS::ACE_HRTimer_Op op)
{
  ACE_TRACE ("ACE_High_Res_Timer::start");
  this->start_ = ACE_High_Res_Timer::gettime (op);
}

ACE_INLINE void
ACE_High_Res_Timer::stop (const ACE_OS::ACE_HRTimer_Op op)
{
  ACE_TRACE ("ACE_High_Res_Timer::stop");
  this->end_ = ACE_High_Res_Timer::gettime (op);
}

ACE_INLINE void
ACE_High_Res_Timer::start_incr (const ACE_OS::ACE_HRTimer_Op op)
{
  ACE_TRACE ("ACE_High_Res_Timer::start_incr");
  this->start_incr_ = ACE_High_Res_Timer::gettime (op);
}

ACE_INLINE void
ACE_High_Res_Timer::stop_incr (const ACE_OS::ACE_HRTimer_Op op)
{
  ACE_TRACE ("ACE_High_Res_Timer::stop_incr");
  this->total_ += ACE_High_Res_Timer::gettime (op) - this->start_incr_;
}

ACE_INLINE void
ACE_High_Res_Timer::elapsed_microseconds (ACE_hrtime_t &usecs) const
{
#if defined (ACE_WIN32)
  // Win32 scale factor is in msec
  // This could give overflow when measuring a long time with a
  // big global_scale_factor() (> 48 days with a 4Ghz tick freq.)
  // To be looked after in the future.
  usecs = (ACE_hrtime_t) (((this->end_ - this->start_) * 1000) /
                          global_scale_factor ());
#else
  usecs =
    (ACE_hrtime_t) ((this->end_ - this->start_) / global_scale_factor ());
#endif



}

ACE_INLINE void
ACE_High_Res_Timer::global_scale_factor (ACE_UINT32 gsf)
{
  global_scale_factor_ = gsf;
}
