// -*- C++ -*-

/**
 *  @file Timer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_TIMER_H
#define TAO_Notify_TIMER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Refcountable.h"

#include "ace/Time_Value.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Event_Handler;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Timer
 *
 * @brief Interface for scheduling timers.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Timer : public TAO_Notify_Refcountable
{
public:
  typedef TAO_Notify_Refcountable_Guard_T< TAO_Notify_Timer > Ptr;
  /// Destructor
  virtual ~TAO_Notify_Timer (){};

  /// Schedule a timer
  virtual long schedule_timer (ACE_Event_Handler *handler,
                               const ACE_Time_Value &delay_time,
                               const ACE_Time_Value &interval) = 0;

  /// Cancel Timer
  virtual int cancel_timer (long timer_id) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_TIMER_H */
