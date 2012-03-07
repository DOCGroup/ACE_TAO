// -*- C++ -*-

//=============================================================================
/**
 *  @file   Time_Policy_Strategy.h
 *
 *  $Id$
 *
 *  @author Martin Corino <mcorino@remedy.nl>
 */
//=============================================================================

#ifndef TIME_POLICY_STRATEGY_H
#define TIME_POLICY_STRATEGY_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if (TAO_HAS_TIME_POLICY == 1)

#include "tao/Versioned_Namespace.h"

#include "ace/Service_Object.h"
#include "ace/Timer_Queuefwd.h"
#include "ace/Time_Policy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Time_Policy_Strategy
 *
 * @brief Abstract base class for time policy strategy services.
 *
 */
class TAO_Export TAO_Time_Policy_Strategy
  : public ACE_Service_Object
{
public:
  virtual ~TAO_Time_Policy_Strategy () {}

  virtual ACE_Timer_Queue * create_timer_queue (void) = 0;

  virtual void destroy_timer_queue (ACE_Timer_Queue *tmq) = 0;

  virtual ACE_Dynamic_Time_Policy_Base * get_time_policy (void) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_TIME_POLICY */

#include /**/ "ace/post.h"

#endif /* TIME_POLICY_STRATEGY_H */
