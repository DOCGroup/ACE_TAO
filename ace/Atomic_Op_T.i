// -*- C++ -*-
//
// $Id$

//
// ACE_Atomic_Op_Ex inline functions
//

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator++ (void)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator++");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, this->value_);
  return ++this->value_;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator+= (const TYPE &rhs)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator+=");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, this->value_);
  return this->value_ += rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator-- (void)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator--");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, this->value_);
  return --this->value_;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator-= (const TYPE &rhs)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator-=");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, this->value_);
  return this->value_ -= rhs;
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
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator== (const TYPE &rhs) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator==");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->mutex_, 0);
  return this->value_ == rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator!= (const TYPE &rhs) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator!=");
  return !(*this == rhs);
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator>= (const TYPE &rhs) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator>=");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->mutex_, 0);
  return this->value_ >= rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator> (const TYPE &rhs) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator>");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->mutex_, 0);
  return this->value_ > rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator<= (const TYPE &rhs) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator<=");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->mutex_, 0);
  return this->value_ <= rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator< (const TYPE &rhs) const
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator<");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->mutex_, 0);
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
ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator= (const TYPE &rhs)
{
// ACE_TRACE ("ACE_Atomic_Op_Ex<ACE_LOCK, TYPE>::operator=");
  ACE_GUARD (ACE_LOCK, ace_mon, this->mutex_);
  this->value_ = rhs;
}

//
// ACE_Atomic_Op inline functions
//

template <class ACE_LOCK, class TYPE> ACE_INLINE void
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator= (const TYPE &i)
{
  this->impl_ = i;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE void
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator= (const ACE_Atomic_Op<ACE_LOCK, TYPE> &rhs)
{
  this->impl_ = rhs.impl_;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator++ (void)
{
  return ++this->impl_;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator++ (int)
{
  return this->impl_++;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator+= (const TYPE &rhs)
{
  return this->impl_ += rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator-- (void)
{
  return --this->impl_;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator-- (int)
{
  return this->impl_--;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator-= (const TYPE &rhs)
{
  return this->impl_ -= rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator== (const TYPE &rhs) const
{
  return this->impl_ == rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator!= (const TYPE &rhs) const
{
  return this->impl_ != rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator>= (const TYPE &rhs) const
{
  return this->impl_ >= rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator> (const TYPE &rhs) const
{
  return this->impl_ > rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator<= (const TYPE &rhs) const
{
  return this->impl_ <= rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE int
ACE_Atomic_Op<ACE_LOCK, TYPE>::operator< (const TYPE &rhs) const
{
  return this->impl_ < rhs;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE
ACE_Atomic_Op<ACE_LOCK, TYPE>::value (void) const
{
  return this->impl_.value ();
}

template <class ACE_LOCK, class TYPE> ACE_INLINE void
ACE_Atomic_Op<ACE_LOCK, TYPE>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  this->impl_.dump ();
#endif /* ACE_HAS_DUMP */
  return;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE ACE_LOCK &
ACE_Atomic_Op<ACE_LOCK, TYPE>::mutex (void)
{
  return this->own_mutex_;
}

template <class ACE_LOCK, class TYPE> ACE_INLINE TYPE &
ACE_Atomic_Op<ACE_LOCK, TYPE>::value_i (void)
{
  return this->impl_.value_i ();
}
