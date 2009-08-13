// -*- C++ -*-

/**
 *  @file Timer_Reactor.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_TIMER_REACTOR_H
#define TAO_Notify_TIMER_REACTOR_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"
#include "orbsvcs/Notify/Timer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Timer_Reactor
 *
 * @brief Reactor::instance based timer. The timers are dispatched by
 *        the main thread.
 */
class TAO_Notify_Serv_Export TAO_Notify_Timer_Reactor : public TAO_Notify_Timer
{
public:
  typedef TAO_Notify_Refcountable_Guard_T< TAO_Notify_Timer_Reactor > Ptr;

  /// Constructor
  TAO_Notify_Timer_Reactor (void);

  /// Destructor
  virtual ~TAO_Notify_Timer_Reactor ();

  /// Schedule a timer
  virtual long schedule_timer (ACE_Event_Handler *handler,
                               const ACE_Time_Value &delay_time,
                               const ACE_Time_Value &interval);

  /// Cancel Timer
  virtual int cancel_timer (long timer_id);

protected:
  /// The instance reactor that we use.
  ACE_Reactor* reactor_;

private:
  /// Release
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_TIMER_REACTOR_H */
