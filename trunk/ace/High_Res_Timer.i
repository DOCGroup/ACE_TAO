/* -*- C++ -*- */
// $Id$

// High_Res_Timer.i

#if defined (ACE_HAS_HI_RES_TIMER)

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
  this->temp_ = ACE_OS::gethrtime ();
}

ACE_INLINE void 
ACE_High_Res_Timer::stop_incr (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::stop_incr");
#if defined (ACE_HAS_LONGLONG_T)
  this->total_ += (ACE_OS::gethrtime () - this->temp_);
#endif /* ACE_HAS_LONGLONG_T */
}
#endif /* ACE_HAS_HI_RES_TIMER */
