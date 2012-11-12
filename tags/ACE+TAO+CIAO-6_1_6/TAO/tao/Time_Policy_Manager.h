// -*- C++ -*-

//=============================================================================
/**
 *  @file   Time_Policy_Manager.h
 *
 *  $Id$
 *
 *  @author Martin Corino <mcorino@remedy.nl>
 */
//=============================================================================

#ifndef TIME_POLICY_MANAGER_H
#define TIME_POLICY_MANAGER_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if (TAO_HAS_TIME_POLICY == 1)

#include "tao/Time_Policy_Strategy.h"

#include "ace/Service_Object.h"
#include "ace/Timer_Queuefwd.h"
#include "ace/Time_Policy.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class Time_Policy_Manager
 *
 * @brief Manager service for time policy strategy services.
 *
 */
class TAO_Export TAO_Time_Policy_Manager
  : public ACE_Service_Object
{
public:
  TAO_Time_Policy_Manager ();
  virtual ~TAO_Time_Policy_Manager ();

  // = Service Configurator hooks.
  /// Dynamic linking hook
  virtual int init (int argc, ACE_TCHAR* argv[]);

  /// Parse svc.conf arguments
  int parse_args (int argc, ACE_TCHAR* argv[]);

  virtual ACE_Timer_Queue * create_timer_queue (void);

  virtual void destroy_timer_queue (ACE_Timer_Queue *tmq);

private:
  enum Time_Policy_Setting
  {
    TAO_OS_TIME_POLICY,
    TAO_HR_TIME_POLICY,
    TAO_DYN_TIME_POLICY
  };

  TAO_SYNCH_MUTEX lock_;

  TAO_Time_Policy_Strategy * time_policy_strategy_;

  Time_Policy_Setting time_policy_setting_;

  ACE_CString time_policy_name_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Time_Policy_Manager)
ACE_FACTORY_DECLARE (TAO, TAO_Time_Policy_Manager)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_TIME_POLICY */

#include /**/ "ace/post.h"

#endif /* TIME_POLICY_MANAGER_H */
