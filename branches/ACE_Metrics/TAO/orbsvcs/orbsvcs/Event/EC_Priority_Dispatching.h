/* -*- C++ -*- */
/**
 *  @file   EC_Priority_Dispatching.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_PRIORITY_DISPATCHING_H
#define TAO_EC_PRIORITY_DISPATCHING_H
#include "ace/pre.h"

#include "EC_Dispatching.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecSchedulerC.h"
#include "sched_event_export.h"

#include "ace/Thread_Manager.h"

class TAO_EC_Dispatching_Task;
class TAO_EC_Event_Channel;

/**
 * @class TAO_EC_Priority_Dispatching
 *
 * @brief Dispatching strategy that minimizes priority inversion.
 *
 * This strategy uses multiple queues, each serviced by a thread
 * at different priority.  This minimizes priority inversion
 * because the consumers at higher priority are serviced before
 * consumers at lower priority.
 * It is more flexible than using the supplier thread to dispatch
 * because it allows high-priority suppliers to push events to
 * low-priority consumers (and vice-versa).
 * It also isolates the supplier threads from the time spent on
 * upcalls to the consumer objects, making the system easier to
 * analyze and schedule.
 */
class TAO_RTSchedEvent_Export TAO_EC_Priority_Dispatching : public TAO_EC_Dispatching
{
public:
  /// The scheduler is used to find the range of priorities and similar
  /// info.
  TAO_EC_Priority_Dispatching (TAO_EC_Event_Channel* ec);

  // = The EC_Dispatching methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual void push (TAO_EC_ProxyPushSupplier* proxy,
                     RtecEventComm::PushConsumer_ptr consumer,
                     const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info
                     ACE_ENV_ARG_DECL);
  virtual void push_nocopy (TAO_EC_ProxyPushSupplier* proxy,
                            RtecEventComm::PushConsumer_ptr consumer,
                            RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info
                            ACE_ENV_ARG_DECL);

private:
  /// Use our own thread manager.
  ACE_Thread_Manager thread_manager_;

  /// The number of active tasks
  int ntasks_;

  /// The tasks..
  TAO_EC_Dispatching_Task** tasks_;

  /// The scheduler
  RtecScheduler::Scheduler_var scheduler_;
};

#if defined (__ACE_INLINE__)
#include "EC_Priority_Dispatching.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_PRIORITY_DISPATCHING_H */
