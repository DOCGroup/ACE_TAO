// -*- C++ -*-

/**
 *  @file   EC_Kokyu_Dispatching.h
 *
 *  $Id$
 *
 *  @author Bryan Thrall (thrall@cs.wustl.edu)
 *
 * Based on previous work by Carlos O'Ryan (coryan@cs.wustl.edu) and
 * Tim Harrison (harrison@cs.wustl.edu) and other members of the DOC group.
 * More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_KOKYU_DISPATCHING_H
#define TAO_EC_KOKYU_DISPATCHING_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_Dispatching.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/Event/rtkokyu_event_export.h"

#include "ace/Thread_Manager.h"

#include "Kokyu/Kokyu.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_EC_Event_Channel_Base;

/**
 * @class TAO_EC_Kokyu_Dispatching
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
class TAO_RTKokyuEvent_Export TAO_EC_Kokyu_Dispatching : public TAO_EC_Dispatching
{
public:
  /// The scheduler is used to find the range of priorities and similar
  /// info.
  TAO_EC_Kokyu_Dispatching (TAO_EC_Event_Channel_Base* ec,
                            int sched_policy,
                            int sched_scope);

  // = The EC_Dispatching methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual void push (TAO_EC_ProxyPushSupplier* proxy,
                     RtecEventComm::PushConsumer_ptr consumer,
                     const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info);
  virtual void push_nocopy (TAO_EC_ProxyPushSupplier* proxy,
                            RtecEventComm::PushConsumer_ptr consumer,
                            RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info);

private:
  ACE_Allocator *allocator_;

  void setup_lanes (void);

  /// The dispatcher
  Kokyu::Dispatcher_Auto_Ptr dispatcher_;

  /// The scheduler
  RtecScheduler::Scheduler_var scheduler_;

  int lanes_setup_;
  int disp_sched_policy_;
  int disp_sched_scope_;
};

// ****************************************************************

class TAO_RTKokyuEvent_Export TAO_EC_Kokyu_Shutdown_Command : public Kokyu::Dispatch_Command
{
public:
  /// Constructor
  TAO_EC_Kokyu_Shutdown_Command (ACE_Allocator *allocator);

  /// Command callback
  virtual int execute (void);

protected:
  //Protected so can't be put on stack; must be dynamically allocated
  virtual ~TAO_EC_Kokyu_Shutdown_Command (void);

};

// ****************************************************************

class TAO_RTKokyuEvent_Export TAO_EC_Kokyu_Push_Command : public Kokyu::Dispatch_Command
{
public:
  /// Constructor
  TAO_EC_Kokyu_Push_Command (TAO_EC_ProxyPushSupplier* proxy,
                             RtecEventComm::PushConsumer_ptr consumer,
                             RtecEventComm::EventSet& event,
                             ACE_Allocator* allocator);

  /// Command callback
  virtual int execute (void);

protected:
  //Protected so can't be put on stack; must be dynamically allocated
  virtual ~TAO_EC_Kokyu_Push_Command (void);

private:
  /// The proxy
  TAO_EC_ProxyPushSupplier* proxy_;

  /// The consumer connected to the proxy when the event was pushed.
  RtecEventComm::PushConsumer_var consumer_;

  /// The event
  RtecEventComm::EventSet event_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Kokyu_Dispatching.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_EC_KOKYU_DISPATCHING_H */
