// -*- C++ -*-

//=============================================================================
/**
 *  @file    Condition.h
 *
 *  $Id$
 *
 *  @author From ACE to TAO by Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CONDITION_H
#define TAO_CONDITION_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Condition_T.h"
#include "ace/Global_Macros.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Time_Value;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Condition
 *
 * @brief Same as to the ACE_Condition variable wrapper
 *
 * This class differs from ACE_Condition in that it uses a
 * TAO_SYNCH_CONDITION instead of ACE_cond_t under the hood to
 * provide blocking.
 */
template <class MUTEX>
class TAO_Condition
{
public:

  /// Useful typedef
  typedef MUTEX LOCK;

  // = Initialiation and termination methods.
  /// Initialize the condition variable.
  TAO_Condition (MUTEX &m);

  /// A default constructor. Since no lock is provided by the user,
  /// one will be created internally.
  TAO_Condition (void);

  /// Implicitly destroy the condition variable.
  ~TAO_Condition (void);

  // = Lock accessors.
  /**
   * Block on condition, or until absolute time-of-day has passed.  If
   * abstime == 0 use "blocking" <wait> semantics.  Else, if <abstime>
   * != 0 and the call times out before the condition is signaled
   * <wait> returns -1 and sets errno to ETIME.
   */
  int wait (const ACE_Time_Value *abstime);

  /// Block on condition.
  int wait (void);

  /**
   * Block on condition or until absolute time-of-day has passed.  If
   * abstime == 0 use "blocking" wait() semantics on the <mutex>
   * passed as a parameter (this is useful if you need to store the
   * <Condition> in shared memory).  Else, if <abstime> != 0 and the
   * call times out before the condition is signaled <wait> returns -1
   * and sets errno to ETIME.
   */
  int wait (MUTEX &mutex, const ACE_Time_Value *abstime = 0);

  /// Signal one waiting thread.
  int signal (void);

  /// Signal *all* waiting threads.
  int broadcast (void);

  // = Utility methods.
  /// Explicitly destroy the condition variable.
  int remove (void);

  /// Returns a reference to the underlying mutex_;
  MUTEX *mutex (void);

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_Condition (const TAO_Condition<MUTEX> &))
  ACE_UNIMPLEMENTED_FUNC (TAO_Condition &operator= (const TAO_Condition<MUTEX> &))

  /// Reference to mutex lock.
  MUTEX *mutex_;

  /// A flag to indicate whether the lock needs to be deleted.
  bool delete_lock_;

  /// Condition variable.
  TAO_SYNCH_CONDITION *cond_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Condition.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Condition.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Condition.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /*TAO_CONDITION_H*/
