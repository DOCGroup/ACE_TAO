// -*- C++ -*-

//=============================================================================
/**
 *  @file   Custom_Time_Policy_Strategy.h
 *
 *  $Id$
 *
 *  @author Martin Corino <mcorino@remedy.nl>
 */
//=============================================================================

#ifndef CUSTOM_TIME_POLICY_STRATEGY_H
#define CUSTOM_TIME_POLICY_STRATEGY_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if (TAO_HAS_TIME_POLICY == 1)

#include "tao/Time_Policy_Strategy.h"

#include "ace/Time_Policy_T.h"
#include "ace/Service_Config.h"

#include "TPTest_Export.h"

/**
 * @class Custom_Time_Policy_Strategy
 *
 */
class TPTEST_Export Custom_Time_Policy_Strategy
  : public TAO_Time_Policy_Strategy
{
public:
  virtual ~Custom_Time_Policy_Strategy ();

  virtual ACE_Timer_Queue * create_timer_queue (void);

  virtual void destroy_timer_queue (ACE_Timer_Queue *tmq);

  virtual ACE_Dynamic_Time_Policy_Base * get_time_policy (void);
};

ACE_STATIC_SVC_DECLARE_EXPORT (TPTEST, Custom_Time_Policy_Strategy)
ACE_FACTORY_DECLARE (TPTEST, Custom_Time_Policy_Strategy)

#endif /* TAO_HAS_TIME_POLICY */

#include /**/ "ace/post.h"

#endif /* CUSTOM_TIME_POLICY_STRATEGY_H */
