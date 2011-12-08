// -*- C++ -*-

//=============================================================================
/**
 *  @file   ORB_Time_Policy.h
 *
 *  $Id$
 *
 *  @author Martin Corino <mcorino@remedy.nl>
 */
//=============================================================================

#ifndef ORB_TIME_POLICY_H
#define ORB_TIME_POLICY_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"
#include "ace/Time_Policy.h"
#include "ace/Time_Policy_T.h"
#include "ace/Countdown_Time_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  /**
   * @class ORB_Time_Policy
   *
   * @brief Implement a time policy class which returns timer according
   *        to the active ORB time policy.
   *
   */
  class TAO_Export ORB_Time_Policy
  {
  public:
    /// Return the current time according to this policy
    ACE_Time_Value operator() () const;

    static void set_time_policy (ACE_Dynamic_Time_Policy_Base const * pol);
    static void reset_time_policy (void);

  private:
    static ACE_Time_Policy_T<ACE_Default_Time_Policy> default_policy_;
    static ACE_Dynamic_Time_Policy_Base const * active_policy_;
  };

  // Declare a countdown timer type based on ORB time policy
  typedef ACE_Countdown_Time_T<ORB_Time_Policy> ORB_Countdown_Time;

}; // TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/ORB_Time_Policy.inl"
#endif /* ! __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ORB_TIME_POLICY_H */
