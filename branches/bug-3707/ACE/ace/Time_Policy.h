#ifndef ACE_TIME_POLICY_H
#define ACE_TIME_POLICY_H
// -*- C++ -*-
/**
 *  @file Time_Policy.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#include /**/ "ace/Time_Value.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Default_Time_Policy
 *
 * @brief Implement the default time policy for ACE.
 *
 * The most common time policy is to simply use
 * ACE_OS::gettimeofday(), this class implements that policy, i.e., it
 * simply calls that function.
 */
class ACE_Export ACE_Default_Time_Policy
{
public:
  /// Return the current time according to this policy
  ACE_Time_Value operator() () const;
};

/**
 * @class ACE_FPointer_Timer_Policy
 *
 * @brief Implement a time policy based on a function pointer.
 *
 * This time policy allows dynamic changes to the source of time by
 * using a function pointer.
 */
class ACE_Export ACE_FPointer_Time_Policy
{
public:
  /**
   * @brief Default constructor uses ACE_OS::gettimeofday()
   *
   * ACE_T requires a default constructor that leaves the
   * policy in a functional state.  Therefore, a null pointer would
   * not be desirable, in other words, we need a non-trivial default
   * constructor.
   */
  ACE_FPointer_Time_Policy();

  /**
   * @typedef FPtr
   *
   * Short-hand for the right type of pointer to function.
   */
  typedef ACE_Time_Value (*FPtr)();

  /**
   * @brief Constructor from a pointer to function.
   *
   * Construct from a pointer to function.
   */
  ACE_FPointer_Time_Policy(FPtr f);

  /// Return the current time according to this policy
  ACE_Time_Value operator()() const;


private:
  FPtr function_;
};


ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Time_Policy.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_TIME_POLICY_H */
