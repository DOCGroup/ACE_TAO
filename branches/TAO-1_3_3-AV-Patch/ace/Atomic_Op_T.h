/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Atomic_Op_T.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@uci.edu>
 */
//=============================================================================

#ifndef ACE_ATOMIC_OP_T_H
#define ACE_ATOMIC_OP_T_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"


/**
 * @class ACE_Atomic_Op_Ex
 *
 * @brief Transparently parameterizes synchronization into basic
 * arithmetic operations.
 *
 * This class is described in an article in the July/August 1994
 * issue of the C++ Report magazine.  It implements a
 * templatized version of the Decorator pattern from the GoF book.
 *
 * ACE_Atomic_Op_Ex objects must be constructed with a reference
 * to an existing lock. A single lock can be shared between
 * multiple ACE_Atomic_Op_Ex objects. If you do not require this
 * ability consider using the ACE_Atomic_Op class instead, which
 * may be able to take advantage of platform-specific
 * optimisations to provide atomic operations without requiring a
 * lock.
 */
template <class ACE_LOCK, class TYPE>
class ACE_Atomic_Op_Ex
{
public:
  // = Initialization methods.

  /// Initialize <value_> to 0.
  ACE_Atomic_Op_Ex (ACE_LOCK &mtx);

  /// Initialize <value_> to c.
  ACE_Atomic_Op_Ex (ACE_LOCK &mtx, const TYPE &c);

  // = Accessors.

  /// Atomically pre-increment <value_>.
  TYPE operator++ (void);

  /// Atomically post-increment <value_>.
  TYPE operator++ (int);

  /// Atomically increment <value_> by rhs.
  TYPE operator+= (const TYPE &rhs);

  /// Atomically pre-decrement <value_>.
  TYPE operator-- (void);

  /// Atomically post-decrement <value_>.
  TYPE operator-- (int);

  /// Atomically decrement <value_> by rhs.
  TYPE operator-= (const TYPE &rhs);

  /// Atomically compare <value_> with rhs.
  int operator== (const TYPE &rhs) const;

  /// Atomically compare <value_> with rhs.
  int operator!= (const TYPE &rhs) const;

  /// Atomically check if <value_> greater than or equal to rhs.
  int operator>= (const TYPE &rhs) const;

  /// Atomically check if <value_> greater than rhs.
  int operator> (const TYPE &rhs) const;

  /// Atomically check if <value_> less than or equal to rhs.
  int operator<= (const TYPE &rhs) const;

  /// Atomically check if <value_> less than rhs.
  int operator< (const TYPE &rhs) const;

  /// Atomically assign rhs to <value_>.
  void operator= (const TYPE &rhs);

  /// Atomically assign <rhs> to <value_>.
  void operator= (const ACE_Atomic_Op_Ex<ACE_LOCK, TYPE> &rhs);

  /// Explicitly return <value_>.
  TYPE value (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  /// Manage copying...
  ACE_Atomic_Op_Ex (const ACE_Atomic_Op_Ex<ACE_LOCK, TYPE> &);

  /**
   * Returns a reference to the underlying <ACE_LOCK>.  This makes it
   * possible to acquire the lock explicitly, which can be useful in
   * some cases if you instantiate the <ACE_Atomic_Op_Ex> with an
   * <ACE_Recursive_Mutex> or <ACE_Process_Mutex>.  NOTE: the right
   * name would be lock_, but HP/C++ will choke on that!
   */
  ACE_LOCK &mutex (void);

  /**
   * Explicitly return <value_> (by reference).  This gives the user
   * full, unrestricted access to the underlying value.  This method
   * will usually be used in conjunction with explicit access to the
   * lock.  Use with care ;-)
   */
  TYPE &value_i (void);

private:
  /// Type of synchronization mechanism.
  ACE_LOCK &mutex_;

  /// Current object decorated by the atomic op.
  TYPE value_;
};

/**
 * @class ACE_Atomic_Op
 *
 * @brief Transparently parameterizes synchronization into basic
 * arithmetic operations.
 *
 * This class is described in an article in the July/August 1994
 * issue of the C++ Report magazine.  It implements a
 * templatized version of the Decorator pattern from the GoF book.
 *
 * Certain platforms may provide a template specialization for
 * ACE_Atomic_Op <ACE_Thread_Mutex, long> that provides optimized
 * atomic integer operations without actually requiring a mutex.
 */
template <class ACE_LOCK, class TYPE>
class ACE_Atomic_Op
{
public:
  /// Initialize <value_> to 0.
  ACE_Atomic_Op (void);

  /// Initialize <value_> to c.
  ACE_Atomic_Op (const TYPE &c);

  /// Manage copying...
  ACE_Atomic_Op (const ACE_Atomic_Op<ACE_LOCK, TYPE> &c);

  /// Atomically assign rhs to <value_>.
  void operator= (const TYPE &rhs);

  /// Atomically assign <rhs> to <value_>.
  void operator= (const ACE_Atomic_Op<ACE_LOCK, TYPE> &rhs);

  /// Atomically pre-increment <value_>.
  TYPE operator++ (void);

  /// Atomically post-increment <value_>.
  TYPE operator++ (int);

  /// Atomically increment <value_> by rhs.
  TYPE operator+= (const TYPE &rhs);

  /// Atomically pre-decrement <value_>.
  TYPE operator-- (void);

  /// Atomically post-decrement <value_>.
  TYPE operator-- (int);

  /// Atomically decrement <value_> by rhs.
  TYPE operator-= (const TYPE &rhs);

  /// Atomically compare <value_> with rhs.
  int operator== (const TYPE &rhs) const;

  /// Atomically compare <value_> with rhs.
  int operator!= (const TYPE &rhs) const;

  /// Atomically check if <value_> greater than or equal to rhs.
  int operator>= (const TYPE &rhs) const;

  /// Atomically check if <value_> greater than rhs.
  int operator> (const TYPE &rhs) const;

  /// Atomically check if <value_> less than or equal to rhs.
  int operator<= (const TYPE &rhs) const;

  /// Atomically check if <value_> less than rhs.
  int operator< (const TYPE &rhs) const;

  /// Explicitly return <value_>.
  TYPE value (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /**
   * Returns a reference to the underlying <ACE_LOCK>.  This makes it
   * possible to acquire the lock explicitly, which can be useful in
   * some cases if you instantiate the <ACE_Atomic_Op> with an
   * <ACE_Recursive_Mutex> or <ACE_Process_Mutex>.
   *
   * NOTE: This member function is deprecated and so may go away in
   * the future. If you need access to the underlying mutex, consider
   * using the ACE_Atomic_Op_Ex template instead.
   */
  ACE_LOCK &mutex (void);

  /**
   * Explicitly return <value_> (by reference).  This gives the user
   * full, unrestricted access to the underlying value.  This method
   * will usually be used in conjunction with explicit access to the
   * lock.  Use with care ;-)
   */
  TYPE &value_i (void);

private:
  /// Type of synchronization mechanism.
  ACE_LOCK own_mutex_;

  /// Underlying atomic op implementation.
  ACE_Atomic_Op_Ex <ACE_LOCK, TYPE> impl_;
};


#if defined (__ACE_INLINE__)
#include "ace/Atomic_Op_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Atomic_Op_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Atomic_Op_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /*ACE_ATOMIC_OP_T_H*/
