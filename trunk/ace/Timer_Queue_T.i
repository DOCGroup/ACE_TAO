/* -*- C++ -*- */
// $Id$

// Timer_Queue_T.i

template <class TYPE, class FUNCTOR, class LOCK> ACE_INLINE void
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::timer_skew (const ACE_Time_Value &skew)
{
  timer_skew_ = skew;
}

template <class TYPE, class FUNCTOR, class LOCK> ACE_INLINE const ACE_Time_Value &
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::timer_skew (void) const
{
  return timer_skew_;
}

template <class TYPE, class FUNCTOR, class LOCK> ACE_INLINE int
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::expire (void)
{
  if (!this->is_empty ())
    return this->expire (this->gettimeofday () + timer_skew_);
  else
    return 0;
}

template <class TYPE, class FUNCTOR, class LOCK> ACE_INLINE void
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::upcall (TYPE &type,
						const void *act,
						const ACE_Time_Value &cur_time)
{
  this->upcall_functor_.timeout (*this, type, act, cur_time);
}


template <class TYPE, class FUNCTOR, class LOCK> ACE_INLINE ACE_Time_Value
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::gettimeofday (void)
{
  // Invoke gettimeofday via pointer to function.
  return gettimeofday_ ();
}

template <class TYPE, class FUNCTOR, class LOCK> ACE_INLINE void
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::gettimeofday (ACE_Time_Value (*gettimeofday)(void))
{
  gettimeofday_ = gettimeofday;
}

template <class TYPE, class FUNCTOR, class LOCK> ACE_INLINE FUNCTOR &
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::upcall_functor (void)
{
  return this->upcall_functor_;
}

