/* -*- C++ -*- */
// $Id$

// High_Res_Timer.i

ACE_INLINE
ACE_High_Res_Timer::ACE_High_Res_Timer (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::ACE_High_Res_Timer");
  this->reset ();
}

ACE_INLINE int
ACE_High_Res_Timer::start (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::start");
  return (int) (this->start_ = ACE_OS::gethrtime ());
}

ACE_INLINE int
ACE_High_Res_Timer::stop (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::stop");
  return (int) (this->end_ = ACE_OS::gethrtime ());
}

ACE_INLINE void 
ACE_High_Res_Timer::elapsed_time (ACE_Time_Value &tv)
{
  tv.sec ((this->end_ - this->start_) / 1000000L);
  tv.usec ((this->end_ - this->start_) % 1000000L);
}

#if defined (ACE_HAS_POSIX_TIME)
ACE_INLINE void
ACE_High_Res_Timer::elapsed_time (timespec_t &elapsed_time)
{
  elapsed_time.tv_sec = (this->end_ - this->start_) / (1000 * 1000 * 1000);
  elapsed_time.tv_nsec = (this->end_ - this->start_) % (1000 * 1000 * 1000);
}
#endif /* ACE_HAS_POSIX_TIME */

ACE_INLINE int 
ACE_High_Res_Timer::start_incr (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::start_incr");
  return (int) (this->start_incr_ = ACE_OS::gethrtime ());
}

ACE_INLINE int 
ACE_High_Res_Timer::stop_incr (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::stop_incr");
  return (int) (this->total_ += (ACE_OS::gethrtime () - this->start_incr_));
}

ACE_INLINE void 
ACE_High_Res_Timer::elapsed_time_incr (ACE_Time_Value &tv)
{
  tv.sec ((this->total_) / 1000000L);
  tv.usec ((this->total_) % 1000000L);
}

ACE_INLINE void
ACE_High_Res_Timer::elapsed_microseconds (hrtime_t &usecs) const
{
  usecs = (this->end_ - this->start_) / 1000;
}
