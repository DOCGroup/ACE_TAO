// -*- C++ -*-

/**
 *  @file Timer_Queue.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_TIMER_QUEUE_H
#define TAO_Notify_TIMER_QUEUE_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#include "orbsvcs/Notify/Timer.h"

#include "ace/Timer_Queue.h"
#include "ace/Timer_Heap.h"
#include "ace/Recursive_Thread_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Timer_Queue
 *
 * @brief ACE_Timer_Queue based timer.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Timer_Queue : public TAO_Notify_Timer
{
public:
  typedef TAO_Notify_Refcountable_Guard_T< TAO_Notify_Timer_Queue > Ptr;

  /// Constuctor
  TAO_Notify_Timer_Queue (void);

  /// Destructor
  virtual ~TAO_Notify_Timer_Queue ();


  /// Schedule a timer
  virtual long schedule_timer (ACE_Event_Handler *handler,
                               const ACE_Time_Value &delay_time,
                               const ACE_Time_Value &interval);

  /// Cancel Timer
  virtual int cancel_timer (long timer_id);

  /// Get the native impl.
  ACE_Timer_Queue& impl (void);

protected:
  /// The Timer Queue
  ACE_Timer_Heap timer_queue_;

private:
  /// Release
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_TIMER_QUEUE_H */
