/* -*- C++ -*- */
// $Id$

// Timer_Queue.i

ACE_INLINE void
ACE_Timer_Queue::timer_skew (const ACE_Time_Value &skew)
{
  timer_skew_ = skew;
}

ACE_INLINE const ACE_Time_Value &
ACE_Timer_Queue::timer_skew (void) const
{
  return timer_skew_;
}

ACE_INLINE int
ACE_Timer_Queue::expire (void)
{
  if (!this->is_empty ())
    return this->expire (this->gettimeofday () + timer_skew_);
  else
    return 0;
}
