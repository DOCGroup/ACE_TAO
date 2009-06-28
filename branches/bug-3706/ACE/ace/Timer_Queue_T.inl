// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_INLINE void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::timer_skew (const ACE_Time_Value &skew)
{
  timer_skew_ = skew;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_INLINE const ACE_Time_Value &
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::timer_skew (void) const
{
  return timer_skew_;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_INLINE int
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::expire (void)
{
  if (!this->is_empty ())
    return this->expire (this->gettimeofday () + timer_skew_);
  else
    return 0;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::dispatch_info (const ACE_Time_Value &cur_time,
                                                           ACE_Timer_Node_Dispatch_Info_T<TYPE> &info)
{
  ACE_TRACE ("ACE_Timer_Queue_T::dispatch_info");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, 0));

  return this->dispatch_info_i (cur_time, info);
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_INLINE void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::upcall (ACE_Timer_Node_Dispatch_Info_T<TYPE> &info,
                                                    const ACE_Time_Value &cur_time)
{
  this->upcall_functor ().timeout (*this,
                                   info.type_,
                                   info.act_,
                                   info.recurring_timer_,
                                   cur_time);
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_INLINE void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::preinvoke (ACE_Timer_Node_Dispatch_Info_T<TYPE> &info,
                                                       const ACE_Time_Value &cur_time,
                                                       const void *&upcall_act)
{
  this->upcall_functor ().preinvoke (*this,
                                     info.type_,
                                     info.act_,
                                     info.recurring_timer_,
                                     cur_time,
                                     upcall_act);
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_INLINE void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::postinvoke (ACE_Timer_Node_Dispatch_Info_T<TYPE> &info,
                                                        const ACE_Time_Value &cur_time,
                                                        const void *upcall_act)
{
  this->upcall_functor ().postinvoke (*this,
                                      info.type_,
                                      info.act_,
                                      info.recurring_timer_,
                                      cur_time,
                                      upcall_act);
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_INLINE ACE_Time_Value
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::gettimeofday (void)
{
  // Invoke gettimeofday via pointer to function.
  return this->gettimeofday_ ();
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_INLINE void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::gettimeofday (ACE_Time_Value (*gettimeofday)(void))
{
  this->gettimeofday_ = gettimeofday;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_INLINE FUNCTOR &
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::upcall_functor (void)
{
  return *this->upcall_functor_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
