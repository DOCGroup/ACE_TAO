// $Id$

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE 
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator++ (void)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator++");
  ACE_Guard<ACE_LOCK> m (this->lock_);
  return ++this->value_;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator+= (const TYPE &i)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator+=");
  ACE_Guard<ACE_LOCK> m (this->lock_);
  return this->value_ += i;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator-- (void)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator--");
  ACE_Guard<ACE_LOCK> m (this->lock_);
  return --this->value_;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator-= (const TYPE &i)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator-=");
  ACE_Guard<ACE_LOCK> m (this->lock_);
  return this->value_ -= i;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE
ACE_Atomic_Op<ACE_LOCK, TYPE>::ACE_Atomic_Op (const ACE_Atomic_Op<ACE_LOCK, TYPE> &rhs)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::ACE_Atomic_Op");
  *this = rhs; // Invoke the assignment operator.
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE 
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator++ (int)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator++");
  ACE_Guard<ACE_LOCK> m (this->lock_);
  return this->value_++;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator-- (int)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator--");
  ACE_Guard<ACE_LOCK> m (this->lock_);
  return this->value_--;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator== (const TYPE &i) const
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator==");
  ACE_Guard<ACE_LOCK> m ((ACE_LOCK &) this->lock_);
  return this->value_ == i;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator>= (const TYPE &i) const
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator>=");
  ACE_Guard<ACE_LOCK> m ((ACE_LOCK &) this->lock_);
  return this->value_ >= i;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator> (const TYPE &rhs) const
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator>");
  ACE_Guard<ACE_LOCK> m ((ACE_LOCK &) this->lock_);
  return this->value_ > rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator<= (const TYPE &rhs) const
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator<=");
  ACE_Guard<ACE_LOCK> m ((ACE_LOCK &) this->lock_);
  return this->value_ <= rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator< (const TYPE &rhs) const
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator<");
  ACE_Guard<ACE_LOCK> m ((ACE_LOCK &) this->lock_);
  return this->value_ < rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE void
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator= (const ACE_Atomic_Op<ACE_LOCK, TYPE> &rhs)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator=");
  if (&rhs == this)
    return; // Avoid deadlock...
  ACE_Guard<ACE_LOCK> m (this->lock_);
  // This will call ACE_Atomic_Op::TYPE(), which will ensure the value
  // of <rhs> is acquired atomically.
  this->value_ = rhs; 
}

template <class ACE_LOCK, class TYPE> ACE_INLINE
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator TYPE () const
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator TYPE");
  ACE_Guard<ACE_LOCK> m ((ACE_LOCK &) this->lock_);
  return this->value_;    
}

template <class ACE_LOCK, class TYPE> ACE_INLINE void
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator= (const TYPE &i)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::operator=");
  ACE_Guard<ACE_LOCK> m (this->lock_);
  this->value_ = i;
}

// These specializations have been added to ACE_Atomic_Op to make the
// implementation faster on Win32 that has OS support for doing this
// quickly through methods like InterlockedIncrement and
// InterlockedDecrement

#if defined (ACE_WIN32)

inline long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator++ (void)
{
  return ::InterlockedIncrement (&this->value_);
}

inline long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator-- (void)
{
  return ::InterlockedDecrement (&this->value_);
}

inline void
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator= (const long &i)
{
  ::InterlockedExchange (&this->value_,
                         i);
}

inline void
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator= (const ACE_Atomic_Op<ACE_Thread_Mutex, long> &rhs)
{
  // This will call ACE_Atomic_Op::TYPE(), which will ensure the value
  // of <rhs> is acquired atomically.
  ::InterlockedExchange (&this->value_,
                         rhs);
}

#if defined (ACE_HAS_INTERLOCKED_EXCHANGEADD)

inline long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator+= (const long &i)
{
  return ::InterlockedExchangeAdd (&this->value_, i);
}

inline long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator-= (const long &i)
{
  return ::InterlockedExchangeAdd (&this->value_, -i);
}

#endif /* ACE_HAS_INTERLOCKED_EXCHANGEADD */

#endif /* ACE_WIN32 */
