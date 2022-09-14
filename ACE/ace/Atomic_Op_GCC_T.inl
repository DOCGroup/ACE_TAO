// -*- C++ -*-
#if defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename T>
ACE_INLINE
ACE_Atomic_Op_GCC<T>::ACE_Atomic_Op_GCC ()
  : value_ (0)
{
}

template <typename T>
ACE_INLINE
ACE_Atomic_Op_GCC<T>::ACE_Atomic_Op_GCC (T c)
  : value_ (c)
{
}

template <typename T>
ACE_INLINE
ACE_Atomic_Op_GCC<T>::ACE_Atomic_Op_GCC (
  const ACE_Atomic_Op_GCC<T> &rhs)
  : value_ (rhs.value_)
{
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::operator++ ()
{
  return __atomic_add_fetch (&value_, 1, __ATOMIC_ACQ_REL);
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::operator++ (int)
{
  return __atomic_fetch_add (&value_, 1, __ATOMIC_ACQ_REL);
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::operator-- ()
{
  return __atomic_sub_fetch (&value_, 1, __ATOMIC_ACQ_REL);
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::operator-- (int)
{
  return __atomic_fetch_sub (&value_, 1, __ATOMIC_ACQ_REL);
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::operator+= (T rhs)
{
  return __atomic_add_fetch (&value_, rhs, __ATOMIC_ACQ_REL);
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::operator-= (T rhs)
{
  return __atomic_sub_fetch (&value_, rhs, __ATOMIC_ACQ_REL);
}

template <typename T>
ACE_INLINE bool
ACE_Atomic_Op_GCC<T>::operator== (T rhs) const
{
  return __atomic_load_n (&value_, __ATOMIC_CONSUME) == rhs;
}

template <typename T>
ACE_INLINE bool
ACE_Atomic_Op_GCC<T>::operator!= (T rhs) const
{
  return __atomic_load_n (&value_, __ATOMIC_CONSUME) != rhs;
}

template <typename T>
ACE_INLINE bool
ACE_Atomic_Op_GCC<T>::operator>= (T rhs) const
{
  return __atomic_load_n (&value_, __ATOMIC_CONSUME) >= rhs;
}

template <typename T>
ACE_INLINE bool
ACE_Atomic_Op_GCC<T>::operator> (T rhs) const
{
  return __atomic_load_n (&value_, __ATOMIC_CONSUME) > rhs;
}

template <typename T>
ACE_INLINE bool
ACE_Atomic_Op_GCC<T>::operator<= (T rhs) const
{
  return __atomic_load_n (&value_, __ATOMIC_CONSUME) <= rhs;
}

template <typename T>
ACE_INLINE bool
ACE_Atomic_Op_GCC<T>::operator< (T rhs) const
{
  return __atomic_load_n (&value_, __ATOMIC_CONSUME) < rhs;
}

template <typename T>
ACE_INLINE ACE_Atomic_Op_GCC<T> &
ACE_Atomic_Op_GCC<T>::operator= (T rhs)
{
  __atomic_store_n (&value_, rhs, __ATOMIC_RELEASE);
  return *this;
}

template <typename T>
ACE_INLINE ACE_Atomic_Op_GCC<T> &
ACE_Atomic_Op_GCC<T>::operator= (
   const ACE_Atomic_Op_GCC<T> &rhs)
{
  __atomic_store_n (&value_, __atomic_load_n (&rhs.value_, __ATOMIC_CONSUME), __ATOMIC_RELEASE);
  return *this;
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::exchange (T newval)
{
  return __atomic_exchange_n (&value_, newval, __ATOMIC_ACQ_REL);
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::value () const
{
  return __atomic_load_n (&value_, __ATOMIC_CONSUME);
}

template <typename T>
ACE_INLINE volatile T &
ACE_Atomic_Op_GCC<T>::value_i ()
{
  return value_;
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_GCC_ATOMIC_BUILTINS */
