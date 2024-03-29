// -*- C++ -*-

//=============================================================================
/**
 *  @file   System_Time_Policy_Strategy.h
 *
 *  @author Martin Corino <mcorino@remedy.nl>
 */
//=============================================================================

#ifndef SYSTEM_TIME_POLICY_STRATEGY_H
#define SYSTEM_TIME_POLICY_STRATEGY_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if (TAO_HAS_TIME_POLICY == 1)

#include "tao/Time_Policy_Strategy.h"

#include "ace/Time_Policy_T.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_System_Time_Policy_Strategy
 *
 * @brief Time policy strategy providing Highres time.
 */
class TAO_Export TAO_System_Time_Policy_Strategy
  : public TAO_Time_Policy_Strategy
{
public:
  virtual ~TAO_System_Time_Policy_Strategy ();

  virtual ACE_Timer_Queue * create_timer_queue ();

  virtual void destroy_timer_queue (ACE_Timer_Queue *tmq);

  virtual ACE_Dynamic_Time_Policy_Base * get_time_policy ();

private:
  static ACE_Time_Policy_T<ACE_System_Time_Policy>  time_policy_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_System_Time_Policy_Strategy)
ACE_FACTORY_DECLARE (TAO, TAO_System_Time_Policy_Strategy)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_TIME_POLICY */

#include /**/ "ace/post.h"

#endif /* SYSTEM_TIME_POLICY_STRATEGY_H */
