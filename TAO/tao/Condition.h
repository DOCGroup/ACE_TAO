/**
 *  @file    Condition.h
 *
 *  @author From ACE to TAO by Balachandran Natarajan <bala@cs.wustl.edu>
 */

#ifndef TAO_CONDITION_H
#define TAO_CONDITION_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Condition_T.h"
#include "ace/Global_Macros.h"

#include <memory>

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
  using LOCK = MUTEX;

  /// @name Initialization and termination
  /// @{

  /// Initialize the condition variable.
  explicit TAO_Condition (MUTEX &m);

  /// A default constructor. Since no lock is provided by the user,
  /// one will be created internally.
  TAO_Condition ();

  /// Implicitly destroy the condition variable.
  ~TAO_Condition ();

  /// @}

  /// @name Wait and signal operations
  /// @{

  /**
   * Block on condition, or until absolute time-of-day has passed. If
   * @a abstime is null, use blocking @c wait() semantics. Otherwise,
   * if @a abstime is not null and the call times out before the
   * condition is signaled, @c wait() returns -1 and sets errno to ETIME.
   */
  int wait (ACE_Time_Value const *abstime);

  /// Block on condition.
  int wait ();

  /**
   * Block on condition or until absolute time-of-day has passed. If
   * @a abstime is null, use blocking @c wait() semantics on @a mutex
   * (this is useful if you need to store @c TAO_Condition in shared
   * memory). Otherwise, if @a abstime is not null and the call times
   * out before the condition is signaled, @c wait() returns -1 and
   * sets errno to ETIME.
   */
  int wait (MUTEX &mutex, ACE_Time_Value const *abstime = nullptr);

  /// Signal one waiting thread.
  int signal ();

  /// Signal *all* waiting threads.
  int broadcast ();

  /// @}

  /// @name Utility methods
  /// @{

  /// Explicitly destroy the condition variable.
  int remove ();

  /// Returns a reference to the underlying mutex_;
  MUTEX *mutex ();

  /// @}

private:
  TAO_Condition (TAO_Condition<MUTEX> const &) = delete;
  TAO_Condition &operator= (TAO_Condition<MUTEX> const &) = delete;
  TAO_Condition (TAO_Condition<MUTEX> &&) = delete;
  TAO_Condition &operator= (TAO_Condition<MUTEX> &&) = delete;

  /// Internally owned mutex, empty when an external mutex is used.
  std::unique_ptr<MUTEX> owned_mutex_ {};

  /// Mutex used by the condition variable.
  MUTEX *mutex_ {};

  /// Condition variable.
  std::unique_ptr<TAO_SYNCH_CONDITION> cond_ {};
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Condition.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Condition.cpp"

#include /**/ "ace/post.h"
#endif /*TAO_CONDITION_H*/
