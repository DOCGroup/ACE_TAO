// $Id$

/* -*- C++ -*- */

template <class TYPE> ACE_INLINE void
ACE_Timer_Node_T<TYPE>::set (const TYPE &type, 
                             const void *a, 
                             const ACE_Time_Value &t, 
                             const ACE_Time_Value &i, 
                             ACE_Timer_Node_T<TYPE> *n, 
                             long timer_id)
{
  this->type_ = type;
  this->act_ = a;
  this->timer_value_ = t;
  this->interval_ = i;
  this->next_ = n;
  this->timer_id_ = timer_id;
}

template <class TYPE> ACE_INLINE void
ACE_Timer_Node_T<TYPE>::set (const TYPE &type, 
                             const void *a, 
                             const ACE_Time_Value &t, 
                             const ACE_Time_Value &i, 
                             ACE_Timer_Node_T<TYPE> *p,
                             ACE_Timer_Node_T<TYPE> *n, 
                             long timer_id)
{
  this->type_ = type;
  this->act_ = a;
  this->timer_value_ = t;
  this->interval_ = i;
  this->prev_ = p;
  this->next_ = n;
  this->timer_id_ = timer_id;
}

template <class TYPE> ACE_INLINE TYPE &
ACE_Timer_Node_T<TYPE>::get_type (void)
{
  return this->type_;
}

template <class TYPE> ACE_INLINE void 
ACE_Timer_Node_T<TYPE>::set_type (TYPE &type)
{
  this->type_ = type;
}

template <class TYPE> ACE_INLINE const void *
ACE_Timer_Node_T<TYPE>::get_act (void) 
{
  return this->act_;
}

template <class TYPE> ACE_INLINE void 
ACE_Timer_Node_T<TYPE>::set_act (void *act)
{
  this->act_ = act;
}

template <class TYPE> ACE_INLINE ACE_Time_Value &
ACE_Timer_Node_T<TYPE>::get_timer_value (void) 
{
  return this->timer_value_;
}

template <class TYPE> ACE_INLINE void 
ACE_Timer_Node_T<TYPE>::set_timer_value (ACE_Time_Value timer_value)
{
  this->timer_value_ = timer_value;
}

template <class TYPE> ACE_INLINE ACE_Time_Value &
ACE_Timer_Node_T<TYPE>::get_interval (void) 
{
  return this->interval_;
}

template <class TYPE> ACE_INLINE void 
ACE_Timer_Node_T<TYPE>::set_interval (ACE_Time_Value interval)
{
  this->interval_ = interval;
}

template <class TYPE> ACE_INLINE ACE_Timer_Node_T<TYPE> *
ACE_Timer_Node_T<TYPE>::get_prev (void) 
{
  return this->prev_;
}

template <class TYPE> ACE_INLINE void 
ACE_Timer_Node_T<TYPE>::set_prev (ACE_Timer_Node_T<TYPE> *prev)
{
  this->prev_ = prev;
}

template <class TYPE> ACE_INLINE ACE_Timer_Node_T<TYPE> *
ACE_Timer_Node_T<TYPE>::get_next (void) 
{
  return this->next_;
}

template <class TYPE> ACE_INLINE void 
ACE_Timer_Node_T<TYPE>::set_next (ACE_Timer_Node_T<TYPE> *next)
{
  this->next_ = next;
}
  
template <class TYPE> ACE_INLINE long 
ACE_Timer_Node_T<TYPE>::get_timer_id (void) 
{
  return this->timer_id_;
}

template <class TYPE> ACE_INLINE void
ACE_Timer_Node_T<TYPE>::set_timer_id (long timer_id)
{
  this->timer_id_ = timer_id;
}

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

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_INLINE void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::upcall (TYPE &type,
						const void *act,
						const ACE_Time_Value &cur_time)
{
  this->upcall_functor ().timeout (*this, type, act, cur_time);
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

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_INLINE ACE_LOCK &
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::mutex (void)
{
  return this->mutex_;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_INLINE FUNCTOR &
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::upcall_functor (void)
{
  return *this->upcall_functor_;
}
