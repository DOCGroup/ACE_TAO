// -*- C++ -*-
//
// $Id$


#if defined (ACE_HAS_BUILTIN_ATOMIC_OP)

ACE_INLINE
ACE_Atomic_Op<ACE_Thread_Mutex, long>::ACE_Atomic_Op (void)
  : value_ (0)
{
}

ACE_INLINE
ACE_Atomic_Op<ACE_Thread_Mutex, long>::ACE_Atomic_Op (long c)
  : value_ (c)
{
}

ACE_INLINE
ACE_Atomic_Op<ACE_Thread_Mutex, long>::ACE_Atomic_Op (
  const ACE_Atomic_Op<ACE_Thread_Mutex, long> &rhs)
  : value_ (rhs.value_)
{
}

ACE_INLINE long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator++ (void)
{
#if defined (WIN32)
  return ::InterlockedIncrement (ACE_const_cast (long *, &this->value_));
#else /* WIN32 */
  return (*increment_fn_) (&this->value_);
#endif /* WIN32 */
}

ACE_INLINE long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator++ (int)
{
  return ++*this - 1;
}

ACE_INLINE long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator-- (void)
{
#if defined (WIN32)
  return ::InterlockedDecrement (ACE_const_cast (long *, &this->value_));
#else /* WIN32 */
  return (*decrement_fn_) (&this->value_);
#endif /* WIN32 */
}

ACE_INLINE long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator-- (int)
{
  return --*this + 1;
}

ACE_INLINE long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator+= (long rhs)
{
#if defined (WIN32) && defined (ACE_HAS_INTERLOCKED_EXCHANGEADD)
  return ::InterlockedExchangeAdd (ACE_const_cast (long *, &this->value_),
                                   rhs) + rhs;
#else /* WIN32 && ACE_HAS_INTERLOCKED_EXCHANGEADD */
  return (*exchange_add_fn_) (&this->value_, rhs) + rhs;
#endif /* WIN32 && ACE_HAS_INTERLOCKED_EXCHANGEADD */
}

ACE_INLINE long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator-= (long rhs)
{
#if defined (WIN32) && defined (ACE_HAS_INTERLOCKED_EXCHANGEADD)
  return ::InterlockedExchangeAdd (ACE_const_cast (long *, &this->value_),
                                   -rhs) - rhs;
#else /* WIN32 && ACE_HAS_INTERLOCKED_EXCHANGEADD */
  return (*exchange_add_fn_) (&this->value_, -rhs) - rhs;
#endif /* WIN32 && ACE_HAS_INTERLOCKED_EXCHANGEADD */
}

ACE_INLINE int
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator== (long rhs) const
{
  return (this->value_ == rhs);
}

ACE_INLINE int
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator!= (long rhs) const
{
  return (this->value_ != rhs);
}

ACE_INLINE int
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator>= (long rhs) const
{
  return (this->value_ >= rhs);
}

ACE_INLINE int
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator> (long rhs) const
{
  return (this->value_ > rhs);
}

ACE_INLINE int
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator<= (long rhs) const
{
  return (this->value_ <= rhs);
}

ACE_INLINE int
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator< (long rhs) const
{
  return (this->value_ < rhs);
}

ACE_INLINE void
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator= (long rhs)
{
#if defined (WIN32)
  ::InterlockedExchange (ACE_const_cast (long *, &this->value_), rhs);
#else /* WIN32 */
  (*exchange_fn_) (&this->value_, rhs);
#endif /* WIN32 */
}

ACE_INLINE void
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator= (
   const ACE_Atomic_Op<ACE_Thread_Mutex, long> &rhs)
{
#if defined (WIN32)
  ::InterlockedExchange (ACE_const_cast (long *, &this->value_), rhs.value_);
#else /* WIN32 */
  (*exchange_fn_) (&this->value_, rhs.value_);
#endif /* WIN32 */
}

ACE_INLINE long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::value (void) const
{
  return this->value_;
}

ACE_INLINE volatile long &
ACE_Atomic_Op<ACE_Thread_Mutex, long>::value_i (void)
{
  return this->value_;
}

#endif /* ACE_HAS_BUILTIN_ATOMIC_OP */
