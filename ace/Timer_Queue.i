/* -*- C++ -*- */
// $Id$

// Timer_Queue.i

template <class TYPE, class FUNCTOR> ACE_INLINE void
ACE_Timer_Queue_T<TYPE, FUNCTOR>::timer_skew (const ACE_Time_Value &skew)
{
  timer_skew_ = skew;
}

template <class TYPE, class FUNCTOR> ACE_INLINE const ACE_Time_Value &
ACE_Timer_Queue_T<TYPE, FUNCTOR>::timer_skew (void) const
{
  return timer_skew_;
}

template <class TYPE, class FUNCTOR> ACE_INLINE int
ACE_Timer_Queue_T<TYPE, FUNCTOR>::expire (void)
{
  if (!this->is_empty ())
    return this->expire (this->gettimeofday () + timer_skew_);
  else
    return 0;
}
