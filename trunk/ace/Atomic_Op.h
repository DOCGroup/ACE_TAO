/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Atomic_Op.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@uci.edu>
 */
//=============================================================================

#ifndef ACE_ATOMIC_OP_H
#define ACE_ATOMIC_OP_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"


// Include the templates here.
#include "ace/Atomic_Op_T.h"

// Determine whether builtin atomic op support is
// available on this platform.
#if defined (ACE_HAS_THREADS)
# if defined (WIN32)
#  if defined (ACE_HAS_INTERLOCKED_EXCHANGEADD)
#   define ACE_HAS_BUILTIN_ATOMIC_OP
#  else /* ACE_HAS_INTERLOCKED_EXCHANGEADD */
    // Inline assembly emulation of InterlockedExchangeAdd
    // is currently only implemented for MSVC (x86 only) and Borland.
#   if (defined (_MSC_VER) && defined (_M_IX86)) || defined (__BORLANDC__)
#    define ACE_HAS_BUILTIN_ATOMIC_OP
#   endif /* _MSC_VER || __BORLANDC__ */
#  endif /* ACE_HAS_INTERLOCKED_EXCHANGEADD */
# elif defined (__GNUC__) && defined (ACE_HAS_PENTIUM)
#  define ACE_HAS_BUILTIN_ATOMIC_OP
# endif /* WIN32 */
#endif /* ACE_HAS_THREADS */

#if defined (ACE_HAS_BUILTIN_ATOMIC_OP)
ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Atomic_Op<ACE_Thread_Mutex, long>
 *
 * @brief Specialization of ACE_Atomic_Op for platforms that
 *        support atomic integer operations.
 *
 * Specialization of ACE_Atomic_Op for platforms that support atomic
 * integer operations.
 */
class ACE_Export ACE_Atomic_Op<ACE_Thread_Mutex, long>
{
public:
  /// Initialize <value_> to 0.
  ACE_Atomic_Op (void);

  /// Initialize <value_> to c.
  ACE_Atomic_Op (long c);

  /// Manage copying...
  ACE_Atomic_Op (const ACE_Atomic_Op<ACE_Thread_Mutex, long> &c);

  /// Atomically pre-increment <value_>.
  long operator++ (void);

  /// Atomically post-increment <value_>.
  long operator++ (int);

  /// Atomically increment <value_> by rhs.
  long operator+= (long rhs);

  /// Atomically pre-decrement <value_>.
  long operator-- (void);

  /// Atomically post-decrement <value_>.
  long operator-- (int);

  /// Atomically decrement <value_> by rhs.
  long operator-= (long rhs);

  /// Atomically compare <value_> with rhs.
  int operator== (long rhs) const;

  /// Atomically compare <value_> with rhs.
  int operator!= (long rhs) const;

  /// Atomically check if <value_> greater than or equal to rhs.
  int operator>= (long rhs) const;

  /// Atomically check if <value_> greater than rhs.
  int operator> (long rhs) const;

  /// Atomically check if <value_> less than or equal to rhs.
  int operator<= (long rhs) const;

  /// Atomically check if <value_> less than rhs.
  int operator< (long rhs) const;

  /// Atomically assign rhs to <value_>.
  void operator= (long rhs);

  /// Atomically assign <rhs> to <value_>.
  void operator= (const ACE_Atomic_Op<ACE_Thread_Mutex, long> &rhs);

  /// Explicitly return <value_>.
  long value (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Explicitly return <value_> (by reference).
  volatile long &value_i (void);

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  /// Used during ACE object manager initialization to optimize the fast
  /// atomic op implementation according to the number of CPUs.
  static void init_functions (void);

private:
  // This function cannot be supported by this template specialization.
  // If you need access to an underlying lock, use the ACE_Atomic_Op_Ex
  // template instead.
  ACE_UNIMPLEMENTED_FUNC (ACE_Thread_Mutex &mutex (void))

  /// Current object decorated by the atomic op.
  volatile long value_;

  // Single-cpu atomic op implementations.
  static long single_cpu_increment (volatile long *value);
  static long single_cpu_decrement (volatile long *value);
  static long single_cpu_exchange (volatile long *value, long rhs);
  static long single_cpu_exchange_add (volatile long *value, long rhs);

  // Multi-cpu atomic op implementations.
  static long multi_cpu_increment (volatile long *value);
  static long multi_cpu_decrement (volatile long *value);
  static long multi_cpu_exchange (volatile long *value, long rhs);
  static long multi_cpu_exchange_add (volatile long *value, long rhs);

  // Pointers to selected atomic op implementations.
  static long (*increment_fn_) (volatile long *);
  static long (*decrement_fn_) (volatile long *);
  static long (*exchange_fn_) (volatile long *, long);
  static long (*exchange_add_fn_) (volatile long *, long);
};
#endif /* ACE_HAS_BUILTIN_ATOMIC_OP */


#if defined (__ACE_INLINE__)
#include "ace/Atomic_Op.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*ACE_ATOMIC_OP_H*/
