/* -*- C++ -*- */
/**
 *  @file Timer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_TIMER_H
#define TAO_NS_TIMER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Time_Value.h"
#include "Refcountable.h"

class ACE_Event_Handler;

/**
 * @class TAO_NS_Timer
 *
 * @brief Interface for scheduling timers.
 *
 */
class TAO_Notify_Export TAO_NS_Timer : public TAO_NS_Refcountable
{
public:
  /// Destructor
  virtual ~TAO_NS_Timer (){};

  /// Schedule a timer
  virtual long schedule_timer (ACE_Event_Handler *handler,
                               const ACE_Time_Value &delay_time,
                               const ACE_Time_Value &interval) = 0;

  /// Cancel Timer
  virtual int cancel_timer (long timer_id) = 0;
};

#include "ace/post.h"
#endif /* TAO_NS_TIMER_H */
