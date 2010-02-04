// -*- C++ -*-
//
// $Id$

#if defined (ACE_HAS_BUILTIN_ATOMIC_OP)

#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
#  include "ace/os_include/os_intrin.h"

#pragma intrinsic (_InterlockedExchange, _InterlockedExchangeAdd, _InterlockedIncrement, _InterlockedDecrement)
#endif /* ACE_HAS_INTRINSIC_INTERLOCKED */

#if defined (ACE_HAS_VXATOMICLIB)
# include <vxAtomicLib.h>
#endif

#if defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
// See http://gcc.gnu.org/onlinedocs/gcc-4.4.3/gcc/Atomic-Builtins.html
# include <stdatomic.h>
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

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
#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
  return ::_InterlockedIncrement (const_cast<long *> (&this->value_));
#elif defined (WIN32)
  return ::InterlockedIncrement (const_cast<long *> (&this->value_));
#elif defined (ACE_HAS_VXATOMICLIB)
  return ::vxAtomicInc (reinterpret_cast <atomic_t*>(const_cast<long *> (&this->value_))) + 1;
#elif defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
  return __sync_add_and_fetch (&this->value_, 1);
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
#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
  return ::_InterlockedDecrement (const_cast<long *> (&this->value_));
#elif defined (WIN32)
  return ::InterlockedDecrement (const_cast<long *> (&this->value_));
#elif defined (ACE_HAS_VXATOMICLIB)
  return ::vxAtomicDec (reinterpret_cast <atomic_t*>(const_cast<long *> (&this->value_))) - 1;
#elif defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
  return __sync_sub_and_fetch (&this->value_, 1);
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
#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
  return ::_InterlockedExchangeAdd (const_cast<long *> (&this->value_),
                                    rhs) + rhs;
#elif defined (WIN32) && defined (ACE_HAS_INTERLOCKED_EXCHANGEADD)
  return ::InterlockedExchangeAdd (const_cast<long *> (&this->value_),
                                   rhs) + rhs;
#elif defined (ACE_HAS_VXATOMICLIB)
  return ::vxAtomicAdd (reinterpret_cast <atomic_t*>(const_cast<long *> (&this->value_)), rhs) + rhs;
#elif defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
  return __sync_add_and_fetch (&this->value_, rhs);
#else /* WIN32 && ACE_HAS_INTERLOCKED_EXCHANGEADD */
  return (*exchange_add_fn_) (&this->value_, rhs) + rhs;
#endif /* WIN32 && ACE_HAS_INTERLOCKED_EXCHANGEADD */
}

ACE_INLINE long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator-= (long rhs)
{
#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
  return ::_InterlockedExchangeAdd (const_cast<long *> (&this->value_),
                                    -rhs) - rhs;
#elif defined (WIN32) && defined (ACE_HAS_INTERLOCKED_EXCHANGEADD)
  return ::InterlockedExchangeAdd (const_cast<long *> (&this->value_),
                                   -rhs) - rhs;
#elif defined (ACE_HAS_VXATOMICLIB)
  return ::vxAtomicSub (reinterpret_cast <atomic_t*>(const_cast<long *> (&this->value_)), rhs) - rhs;
#elif defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
  return __sync_sub_and_fetch (&this->value_, rhs);
#else /* WIN32 && ACE_HAS_INTERLOCKED_EXCHANGEADD */
  return (*exchange_add_fn_) (&this->value_, -rhs) - rhs;
#endif /* WIN32 && ACE_HAS_INTERLOCKED_EXCHANGEADD */
}

ACE_INLINE bool
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator== (long rhs) const
{
  return (this->value_ == rhs);
}

ACE_INLINE bool
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator!= (long rhs) const
{
  return (this->value_ != rhs);
}

ACE_INLINE bool
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator>= (long rhs) const
{
  return (this->value_ >= rhs);
}

ACE_INLINE bool
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator> (long rhs) const
{
  return (this->value_ > rhs);
}

ACE_INLINE bool
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator<= (long rhs) const
{
  return (this->value_ <= rhs);
}

ACE_INLINE bool
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator< (long rhs) const
{
  return (this->value_ < rhs);
}

ACE_INLINE ACE_Atomic_Op<ACE_Thread_Mutex, long> &
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator= (long rhs)
{
#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
  ::_InterlockedExchange (const_cast<long *> (&this->value_), rhs);
#elif defined (WIN32)
  ::InterlockedExchange (const_cast<long *> (&this->value_), rhs);
#elif defined (ACE_HAS_VXATOMICLIB)
  ::vxAtomicSet (reinterpret_cast <atomic_t*>(const_cast<long *> (&this->value_)), rhs);
#elif defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
  __sync_lock_test_and_set (&this->value_, rhs);
#else /* WIN32 */
  (*exchange_fn_) (&this->value_, rhs);
#endif /* WIN32 */
  return *this;
}

ACE_INLINE ACE_Atomic_Op<ACE_Thread_Mutex, long> &
ACE_Atomic_Op<ACE_Thread_Mutex, long>::operator= (
   const ACE_Atomic_Op<ACE_Thread_Mutex, long> &rhs)
{
#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
  ::_InterlockedExchange (const_cast<long *> (&this->value_), rhs.value_);
#elif defined (WIN32)
  ::InterlockedExchange (const_cast<long *> (&this->value_), rhs.value_);
#elif defined (ACE_HAS_VXATOMICLIB)
  ::vxAtomicSet (reinterpret_cast <atomic_t*>(const_cast<long *> (&this->value_)), rhs.value_);
#elif defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
  __sync_lock_test_and_set (&this->value_, rhs.value_);
#else /* WIN32 */
  (*exchange_fn_) (&this->value_, rhs.value_);
#endif /* WIN32 */
  return *this;
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


ACE_INLINE
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::ACE_Atomic_Op (void)
  : value_ (0)
{
}

ACE_INLINE
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::ACE_Atomic_Op (unsigned long c)
  : value_ (c)
{
}

ACE_INLINE
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::ACE_Atomic_Op (
  const ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long> &rhs)
  : value_ (rhs.value_)
{
}

ACE_INLINE unsigned long
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator++ (void)
{
#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
  return static_cast<unsigned long> (::_InterlockedIncrement (const_cast<long *> (reinterpret_cast<volatile long *>(&this->value_))));
#elif defined (WIN32)
  return static_cast<unsigned long> (::InterlockedIncrement (const_cast<long *> (reinterpret_cast<volatile long *>(&this->value_))));
#elif defined (ACE_HAS_VXATOMICLIB)
  return static_cast<unsigned long> (::vxAtomicInc (reinterpret_cast <atomic_t*>(const_cast<long *> (reinterpret_cast<volatile long *>(&this->value_))))) + 1;
#elif defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
  return __sync_add_and_fetch (&this->value_, 1);
#else /* WIN32 */
  return static_cast<unsigned long> ((*increment_fn_) (reinterpret_cast<volatile long *> (&this->value_)));
#endif /* WIN32 */
}

ACE_INLINE unsigned long
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator++ (int)
{
  return ++*this - 1;
}

ACE_INLINE unsigned long
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator-- (void)
{
#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
  return static_cast<unsigned long> (::_InterlockedDecrement (const_cast<long *> (reinterpret_cast<volatile long *>(&this->value_))));
#elif defined (WIN32)
  return static_cast<unsigned long> (::InterlockedDecrement (const_cast<long *> (reinterpret_cast<volatile long *>(&this->value_))));
#elif defined (ACE_HAS_VXATOMICLIB)
  return static_cast<unsigned long> (::vxAtomicDec (reinterpret_cast <atomic_t*>(const_cast<long *> (reinterpret_cast<volatile long *>(&this->value_))))) - 1;
#elif defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
  return __sync_sub_and_fetch (&this->value_, 1);
#else /* WIN32 */
  return static_cast<unsigned long> ((*decrement_fn_) (reinterpret_cast<volatile long *> (&this->value_)));
#endif /* WIN32 */
}

ACE_INLINE unsigned long
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator-- (int)
{
  return --*this + 1;
}

ACE_INLINE unsigned long
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator+= (unsigned long rhs)
{
#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
  return static_cast<unsigned long> (::_InterlockedExchangeAdd (const_cast<long *> (reinterpret_cast <volatile long *>(&this->value_)),
                                   rhs)) + rhs;
#elif defined (WIN32) && defined (ACE_HAS_INTERLOCKED_EXCHANGEADD)
  return static_cast<unsigned long> (::InterlockedExchangeAdd (const_cast<long *> (reinterpret_cast <volatile long *>(&this->value_)),
                                   rhs)) + rhs;
#elif defined (ACE_HAS_VXATOMICLIB)
  return static_cast<unsigned long> (::vxAtomicAdd (reinterpret_cast <atomic_t*>(const_cast<long *> (reinterpret_cast<volatile long *>(&this->value_))), rhs)) + rhs;
#elif defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
  return __sync_add_and_fetch (&this->value_, rhs);
#else /* WIN32 && ACE_HAS_INTERLOCKED_EXCHANGEADD */
  return static_cast<unsigned long> ((*exchange_add_fn_) (reinterpret_cast<volatile long *> (&this->value_), rhs)) + rhs;
#endif /* WIN32 && ACE_HAS_INTERLOCKED_EXCHANGEADD */
}

ACE_INLINE unsigned long
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator-= (unsigned long rhs)
{
#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
  return static_cast<unsigned long> (::_InterlockedExchangeAdd (const_cast<long *> (reinterpret_cast<volatile long *>(&this->value_)),
                                   -static_cast<long>(rhs))) - rhs;
#elif defined (WIN32) && defined (ACE_HAS_INTERLOCKED_EXCHANGEADD)
  return static_cast<unsigned long> (::InterlockedExchangeAdd (const_cast<long *> (reinterpret_cast<volatile long *>(&this->value_)),
                                   -static_cast<long>(rhs))) - rhs;
#elif defined (ACE_HAS_VXATOMICLIB)
  return static_cast<unsigned long> (::vxAtomicSub (reinterpret_cast <atomic_t*>(const_cast<long *> (reinterpret_cast<volatile long *>(&this->value_))), rhs)) - rhs;
#elif defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
  return __sync_sub_and_fetch (&this->value_, rhs);
#else /* WIN32 && ACE_HAS_INTERLOCKED_EXCHANGEADD */
  long l_rhs = static_cast<long> (rhs);
  return static_cast<unsigned long> ((*exchange_add_fn_) (reinterpret_cast<volatile long *> (&this->value_), -l_rhs)) - rhs;
#endif /* WIN32 && ACE_HAS_INTERLOCKED_EXCHANGEADD */
}

ACE_INLINE bool
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator== (unsigned long rhs) const
{
  return (this->value_ == rhs);
}

ACE_INLINE bool
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator!= (unsigned long rhs) const
{
  return (this->value_ != rhs);
}

ACE_INLINE bool
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator>= (unsigned long rhs) const
{
  return (this->value_ >= rhs);
}

ACE_INLINE bool
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator> (unsigned long rhs) const
{
  return (this->value_ > rhs);
}

ACE_INLINE bool
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator<= (unsigned long rhs) const
{
  return (this->value_ <= rhs);
}

ACE_INLINE bool
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator< (unsigned long rhs) const
{
  return (this->value_ < rhs);
}

ACE_INLINE ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long> &
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator= (unsigned long rhs)
{
#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
  ::_InterlockedExchange (const_cast<long *> (reinterpret_cast<volatile long*> (&this->value_)), rhs);
#elif defined (WIN32)
  ::InterlockedExchange (const_cast<long *> (reinterpret_cast<volatile long*> (&this->value_)), rhs);
#elif defined (ACE_HAS_VXATOMICLIB)
  ::vxAtomicSet (reinterpret_cast <atomic_t*>(const_cast<long *> (reinterpret_cast<volatile long*> (&this->value_))), rhs);
#elif defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
  __sync_lock_test_and_set (&this->value_, rhs);
#else /* WIN32 */
  (*exchange_fn_) (reinterpret_cast<volatile long *> (&this->value_), rhs);
#endif /* WIN32 */
  return *this;
}

ACE_INLINE ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long> &
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::operator= (
   const ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long> &rhs)
{
#if defined (ACE_HAS_INTRINSIC_INTERLOCKED)
  ::_InterlockedExchange (const_cast<long *> (reinterpret_cast<volatile long*> (&this->value_)), rhs.value_);
#elif defined (WIN32)
  ::InterlockedExchange (const_cast<long *> (reinterpret_cast<volatile long*> (&this->value_)), rhs.value_);
#elif defined (ACE_HAS_VXATOMICLIB)
  ::vxAtomicSet (reinterpret_cast <atomic_t*>(const_cast<long *> (reinterpret_cast<volatile long*> (&this->value_))), rhs.value_);
#elif defined (ACE_HAS_GCC_ATOMIC_BUILTINS) && (ACE_HAS_GCC_ATOMIC_BUILTINS == 1)
  __sync_lock_test_and_set (&this->value_, rhs.value_);
#else /* WIN32 */
  (*exchange_fn_) (reinterpret_cast<volatile long *> (&this->value_), rhs.value_);
#endif /* WIN32 */
  return *this;
}

ACE_INLINE unsigned long
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::value (void) const
{
  return this->value_;
}

ACE_INLINE volatile unsigned long &
ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>::value_i (void)
{
  return this->value_;
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_BUILTIN_ATOMIC_OP */
