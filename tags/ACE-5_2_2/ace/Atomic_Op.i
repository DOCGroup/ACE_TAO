// -*- C++ -*-
// $Id$

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator++ (void)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator++");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, this->value_);
  return ++this->value_;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator+= (const TYPE &i)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator+=");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, this->value_);
  return this->value_ += i;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator-- (void)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator--");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, this->value_);
  return --this->value_;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator-= (const TYPE &i)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator-=");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, this->value_);
  return this->value_ -= i;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::ACE_Atomic_Op_Ex (const ACE_Atomic_Op_Ex<ACE_LOCK, TYPE> &rhs)
  : mutex_ (rhs.mutex_)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::ACE_Atomic_Op_Ex");
  *this = rhs; // Invoke the assignment operator.
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator++ (int)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator++");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, this->value_);
  return this->value_++;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator-- (int)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator--");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, this->value_);
  return this->value_--;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator== (const TYPE &i) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator==");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->mutex_, -1);
  return this->value_ == i;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator!= (const TYPE &i) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator!=");
  return !(*this == i);
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator>= (const TYPE &i) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator>=");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->mutex_, -1);
  return this->value_ >= i;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator> (const TYPE &rhs) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator>");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->mutex_, -1);
  return this->value_ > rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator<= (const TYPE &rhs) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator<=");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->mutex_, -1);
  return this->value_ <= rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator< (const TYPE &rhs) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator<");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->mutex_, -1);
  return this->value_ < rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE void
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator= (const ACE_Atomic_Op_Ex<ACE_LOCK, TYPE> &rhs)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator=");
  if (&rhs == this)
    return; // Avoid deadlock...
  ACE_GUARD (ACE_LOCK, ace_mon, this->mutex_);
  // This will call ACE_Atomic_Op_Ex::TYPE(), which will ensure the value
  // of <rhs> is acquired atomically.

  this->value_ = rhs.value ();
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::value (void) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::value");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->mutex_, this->value_);
  return this->value_;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE &
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::value_i (void)
{
  // Explicitly return <value_> (by reference).  This gives the user
  // full, unrestricted access to the underlying value.  This method
  // will usually be used in conjunction with explicit access to the
  // lock.  Use with care ;-)
  return this->value_;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE void
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator= (const TYPE &i)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator=");
  ACE_GUARD (ACE_LOCK, ace_mon, (ACE_LOCK &) this->mutex_);
  this->value_ = i;
}

//
// ACE_Atomic_Op inline functions
//

template <class ACE_LOCK, class TYPE> ACE_INLINE void
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator= (const TYPE &i)
{
   ACE_Atomic_Op_Ex <ACE_LOCK,TYPE> ::operator= (i);
}

template <class ACE_LOCK, class TYPE> ACE_INLINE void
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator= (const ACE_Atomic_Op_Ex<ACE_LOCK, TYPE> &rhs)
{
   ACE_Atomic_Op_Ex <ACE_LOCK,TYPE> ::operator= (rhs);
}

// These specializations have been added to ACE_Atomic_Op_Ex to make the
// implementation faster on Win32 that has OS support for doing this
// quickly through methods like InterlockedIncrement and
// InterlockedDecrement

#if defined (ACE_WIN32)

// FUZZ: disable check_for_inline

ACE_TEMPLATE_SPECIALIZATION
inline long
ACE_Atomic_Op_Ex<ACE_Thread_Mutex, long>::operator++ (void)
{
  return ::InterlockedIncrement (&this->value_);
}

ACE_TEMPLATE_SPECIALIZATION
inline long
ACE_Atomic_Op_Ex<ACE_Thread_Mutex, long>::operator-- (void)
{
  return ::InterlockedDecrement (&this->value_);
}

ACE_TEMPLATE_SPECIALIZATION
inline void
ACE_Atomic_Op_Ex<ACE_Thread_Mutex, long>::operator= (const long &i)
{
  ::InterlockedExchange (&this->value_,
                         i);
}

ACE_TEMPLATE_SPECIALIZATION
inline void
ACE_Atomic_Op_Ex<ACE_Thread_Mutex, long>::operator= (const ACE_Atomic_Op_Ex<ACE_Thread_Mutex, long> &rhs)
{
  ::InterlockedExchange (&this->value_,
                         rhs.value ());
}

#if defined (ACE_HAS_INTERLOCKED_EXCHANGEADD)

ACE_TEMPLATE_SPECIALIZATION
inline long
ACE_Atomic_Op_Ex<ACE_Thread_Mutex, long>::operator+= (const long &i)
{
  return ::InterlockedExchangeAdd (&this->value_, i);
}

ACE_TEMPLATE_SPECIALIZATION
inline long
ACE_Atomic_Op_Ex<ACE_Thread_Mutex, long>::operator-= (const long &i)
{
  return ::InterlockedExchangeAdd (&this->value_, -i);
}

#endif /* ACE_HAS_INTERLOCKED_EXCHANGEADD */

#endif /* ACE_WIN32 */
