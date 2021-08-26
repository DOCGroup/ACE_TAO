// -*- C++ -*-
#if defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename T>
ACE_INLINE
ACE_Atomic_Op_GCC<T>::ACE_Atomic_Op_GCC (void)
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
ACE_Atomic_Op_GCC<T>::operator++ (void)
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_add_fetch (&value_, 1, __ATOMIC_ACQ_REL);
#else
  return __sync_add_and_fetch (&value_, 1);
#endif
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::operator++ (int)
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_fetch_add (&value_, 1, __ATOMIC_ACQ_REL);
#else
  return __sync_fetch_and_add (&value_, 1);
#endif
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::operator-- (void)
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_sub_fetch (&value_, 1, __ATOMIC_ACQ_REL);
#else
  return __sync_sub_and_fetch (&value_, 1);
#endif
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::operator-- (int)
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_fetch_sub (&value_, 1, __ATOMIC_ACQ_REL);
#else
  return __sync_fetch_and_sub (&value_, 1);
#endif
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::operator+= (T rhs)
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_add_fetch (&value_, rhs, __ATOMIC_ACQ_REL);
#else
  return __sync_add_and_fetch (&value_, rhs);
#endif
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::operator-= (T rhs)
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_sub_fetch (&value_, rhs, __ATOMIC_ACQ_REL);
#else
  return __sync_sub_and_fetch (&value_, rhs);
#endif
}

template <typename T>
ACE_INLINE bool
ACE_Atomic_Op_GCC<T>::operator== (T rhs) const
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_load_n (&value_, __ATOMIC_CONSUME) == rhs;
#else
  __sync_synchronize();
  return value_ == rhs;
#endif
}

template <typename T>
ACE_INLINE bool
ACE_Atomic_Op_GCC<T>::operator!= (T rhs) const
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_load_n (&value_, __ATOMIC_CONSUME) != rhs;
#else
  __sync_synchronize();
  return value_ != rhs;
#endif
}

template <typename T>
ACE_INLINE bool
ACE_Atomic_Op_GCC<T>::operator>= (T rhs) const
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_load_n (&value_, __ATOMIC_CONSUME) >= rhs;
#else
  __sync_synchronize();
  return value_ >= rhs;
#endif
}

template <typename T>
ACE_INLINE bool
ACE_Atomic_Op_GCC<T>::operator> (T rhs) const
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_load_n (&value_, __ATOMIC_CONSUME) > rhs;
#else
  __sync_synchronize();
  return value_ > rhs;
#endif
}

template <typename T>
ACE_INLINE bool
ACE_Atomic_Op_GCC<T>::operator<= (T rhs) const
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_load_n (&value_, __ATOMIC_CONSUME) <= rhs;
#else
  __sync_synchronize();
  return value_ <= rhs;
#endif
}

template <typename T>
ACE_INLINE bool
ACE_Atomic_Op_GCC<T>::operator< (T rhs) const
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_load_n (&value_, __ATOMIC_CONSUME) < rhs;
#else
  __sync_synchronize();
  return value_ < rhs;
#endif
}

template <typename T>
ACE_INLINE ACE_Atomic_Op_GCC<T> &
ACE_Atomic_Op_GCC<T>::operator= (T rhs)
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  __atomic_store_n (&value_, rhs, __ATOMIC_RELEASE);
#else
  (void) __sync_lock_test_and_set (&value_, rhs);
#endif
  return *this;
}

template <typename T>
ACE_INLINE ACE_Atomic_Op_GCC<T> &
ACE_Atomic_Op_GCC<T>::operator= (
   const ACE_Atomic_Op_GCC<T> &rhs)
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  __atomic_store_n (&value_, __atomic_load_n (&rhs.value_, __ATOMIC_CONSUME), __ATOMIC_RELEASE);
#else
  (void) __sync_lock_test_and_set (&value_, rhs.value_);
#endif
  return *this;
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::exchange (T newval)
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_exchange_n (&value_, newval, __ATOMIC_ACQ_REL);
#else
  return __sync_val_compare_and_swap (&value_, value_, newval);
#endif
}

template <typename T>
ACE_INLINE T
ACE_Atomic_Op_GCC<T>::value (void) const
{
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
  return __atomic_load_n (&value_, __ATOMIC_CONSUME);
#else
  __sync_synchronize();
  return value_;
#endif
}

template <typename T>
ACE_INLINE volatile T &
ACE_Atomic_Op_GCC<T>::value_i (void)
{
  return value_;
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_GCC_ATOMIC_BUILTINS */
