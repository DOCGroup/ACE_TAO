/* -*- C++ -*- */
/**
 *  @file Timer_Queue.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_TIMER_QUEUE_H
#define TAO_Notify_TIMER_QUEUE_H
#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Timer.h"

#include "ace/Timer_Queue.h"
#include "ace/Timer_Heap.h"

/**
 * @class TAO_Notify_Timer_Queue
 *
 * @brief ACE_Timer_Queue based timer.
 *
 */
class TAO_Notify_Export TAO_Notify_Timer_Queue : public TAO_Notify_Timer
{
public:
  /// Constuctor
  TAO_Notify_Timer_Queue (void);

  /// Destructor
  virtual ~TAO_Notify_Timer_Queue ();

  /// Release
  virtual void release (void);

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
};

#if defined (__ACE_INLINE__)
#include "Timer_Queue.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_TIMER_QUEUE_H */
