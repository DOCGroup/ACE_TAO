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
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class ACE_Atomic_Op_Ex
 *
 * @brief Transparently parameterizes synchronization into basic
 * arithmetic operations.
 *
 * This class is described in an article in the July/August 1994
 * issue of the C++ Report magazine.  It implements a
 * templatized version of the Decorator pattern from the GoF book.
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

  /// Atomically increment <value_> by i.
  TYPE operator+= (const TYPE &i);

  /// Atomically pre-decrement <value_>.
  TYPE operator-- (void);

  /// Atomically post-decrement <value_>.
  TYPE operator-- (int);

  /// Atomically decrement <value_> by i.
  TYPE operator-= (const TYPE &i);

  /// Atomically compare <value_> with i.
  int operator== (const TYPE &i) const;

  /// Atomically compare <value_> with i.
  int operator!= (const TYPE &i) const;

  /// Atomically check if <value_> greater than or equal to i.
  int operator>= (const TYPE &i) const;

  /// Atomically check if <value_> greater than i.
  int operator> (const TYPE &rhs) const;

  /// Atomically check if <value_> less than or equal to i.
  int operator<= (const TYPE &rhs) const;

  /// Atomically check if <value_> less than i.
  int operator< (const TYPE &rhs) const;

  /// Atomically assign i to <value_>.
  void operator= (const TYPE &i);

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
   * some cases if you instantiate the <ACE_Atomic_Op> with an
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

template <class ACE_LOCK, class TYPE>
class ACE_Atomic_Op : public ACE_Atomic_Op_Ex <ACE_LOCK, TYPE>
{
public:
  /// Initialize <value_> to 0.
  ACE_Atomic_Op (void);

  /// Initialize <value_> to c.
  ACE_Atomic_Op (const TYPE &c);

  /// Manage copying...
  ACE_Atomic_Op (const ACE_Atomic_Op<ACE_LOCK, TYPE> &);

  /// Atomically assign i to <value_>.
  void operator= (const TYPE &i);

  /// Atomically assign <rhs> to <value_>.
  void operator= (const ACE_Atomic_Op_Ex<ACE_LOCK, TYPE> &rhs);

private:
  /// Type of synchronization mechanism.
  ACE_LOCK own_mutex_;
};


#if defined (__ACE_INLINE__)
// On non-Win32 platforms, this code will be inlined
#if !defined (ACE_WIN32)
#include "ace/Atomic_Op.i"
#endif /* !ACE_WIN32 */
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)

#include "Atomic_Op.cpp"
// On Win32 platforms, this code will be included as template source
// code and will not be inlined. Therefore, we first turn off
// ACE_INLINE, set it to be nothing, include the code, and then turn
// ACE_INLINE back to its original setting. All this nonsense is
// necessary, since the generic template code that needs to be
// specialized cannot be inlined, else the compiler will ignore the
// specialization code. Also, the specialization code *must* be
// inlined or the compiler will ignore the specializations.
#if defined (ACE_WIN32)
#undef ACE_INLINE
#define ACE_INLINE
#include "ace/Atomic_Op.i"
#undef ACE_INLINE
#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#else
#define ACE_INLINE
#endif /* __ACE_INLINE__ */
#endif /* ACE_WIN32 */
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */


#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Atomic_Op.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /*ACE_ATOMIC_OP_H*/
