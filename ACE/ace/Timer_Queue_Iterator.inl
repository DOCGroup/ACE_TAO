// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class TYPE> ACE_INLINE void
ACE_Timer_Node_T<TYPE>::set (TYPE const &type,
                             void const *a,
                             ACE_Time_Value const &t,
                             ACE_Time_Value const &i,
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
ACE_Timer_Node_T<TYPE>::set (TYPE const &type,
                             void const *a,
                             ACE_Time_Value const &t,
                             ACE_Time_Value const &i,
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
ACE_Timer_Node_T<TYPE>::get_type ()
{
  return this->type_;
}

template <class TYPE> ACE_INLINE void
ACE_Timer_Node_T<TYPE>::set_type (TYPE const &type)
{
  this->type_ = type;
}

template <class TYPE> ACE_INLINE void const *
ACE_Timer_Node_T<TYPE>::get_act () const
{
  return this->act_;
}

template <class TYPE> ACE_INLINE void
ACE_Timer_Node_T<TYPE>::set_act (void const *act)
{
  this->act_ = act;
}

template <class TYPE> ACE_INLINE ACE_Time_Value const &
ACE_Timer_Node_T<TYPE>::get_timer_value () const
{
  return this->timer_value_;
}

template <class TYPE> ACE_INLINE void
ACE_Timer_Node_T<TYPE>::set_timer_value (ACE_Time_Value const &timer_value)
{
  this->timer_value_ = timer_value;
}

template <class TYPE> ACE_INLINE ACE_Time_Value const &
ACE_Timer_Node_T<TYPE>::get_interval () const
{
  return this->interval_;
}

template <class TYPE> ACE_INLINE void
ACE_Timer_Node_T<TYPE>::set_interval (ACE_Time_Value const &interval)
{
  this->interval_ = interval;
}

template <class TYPE> ACE_INLINE ACE_Timer_Node_T<TYPE> *
ACE_Timer_Node_T<TYPE>::get_prev () const
{
  return this->prev_;
}

template <class TYPE> ACE_INLINE void
ACE_Timer_Node_T<TYPE>::set_prev (ACE_Timer_Node_T<TYPE> *prev)
{
  this->prev_ = prev;
}

template <class TYPE> ACE_INLINE ACE_Timer_Node_T<TYPE> *
ACE_Timer_Node_T<TYPE>::get_next () const
{
  return this->next_;
}

template <class TYPE> ACE_INLINE void
ACE_Timer_Node_T<TYPE>::set_next (ACE_Timer_Node_T<TYPE> *next)
{
  this->next_ = next;
}

template <class TYPE> ACE_INLINE long
ACE_Timer_Node_T<TYPE>::get_timer_id () const
{
  return this->timer_id_;
}

template <class TYPE> ACE_INLINE void
ACE_Timer_Node_T<TYPE>::set_timer_id (long timer_id)
{
  this->timer_id_ = timer_id;
}

template <class TYPE> ACE_INLINE void
ACE_Timer_Node_T<TYPE>::get_dispatch_info (ACE_Timer_Node_Dispatch_Info_T<TYPE> &info) const
{
  // Yes, do a copy
  info.type_ = this->type_;
  info.act_  = this->act_;
  info.recurring_timer_ =
    this->interval_ > ACE_Time_Value::zero;
}

ACE_END_VERSIONED_NAMESPACE_DECL
