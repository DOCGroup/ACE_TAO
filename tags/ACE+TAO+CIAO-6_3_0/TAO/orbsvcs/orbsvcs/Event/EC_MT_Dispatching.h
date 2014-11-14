// -*- C++ -*-

/**
 *  @file   EC_MT_Dispatching.h
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

#ifndef TAO_EC_MT_DISPATCHING_H
#define TAO_EC_MT_DISPATCHING_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_Dispatching.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/EC_Dispatching_Task.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_EC_MT_Dispatching
 *
 * @brief Dispatching strategy that minimizes mt inversion.
 *
 * This strategy uses a single queue, serviced by one or more
 * threads.  It's main purpose is to decouple the suppliers from
 * the client execution time, specially in the collocated case.
 */
class TAO_RTEvent_Serv_Export TAO_EC_MT_Dispatching : public TAO_EC_Dispatching
{
public:
  /// Constructor
  /// It will create @a nthreads servicing threads...
  TAO_EC_MT_Dispatching (int nthreads,
                         int thread_creation_flags,
                         int thread_priority,
                         int force_activate,
                         TAO_EC_Queue_Full_Service_Object* queue_full_service_object_name);

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
  /// Use our own thread manager.
  ACE_Thread_Manager thread_manager_;

  /// The number of active tasks
  int nthreads_;

  /// The flags (THR_BOUND, THR_NEW_LWP, etc.) used to create the
  /// dispatching threads.
  int thread_creation_flags_;

  /// The priority of the dispatching threads.
  int thread_priority_;

  /// If activation at the requested priority fails then we fallback on
  /// the defaults for thread activation.
  int force_activate_;

  /// The dispatching task
  TAO_EC_Dispatching_Task task_;

  /// Synchronize access to internal data
  TAO_SYNCH_MUTEX lock_;

  /// Are the threads running?
  int active_;

  /// Service Object information
  // TAO_EC_Queue_Full_Service_Object* queue_full_service_object_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_EC_MT_DISPATCHING_H */
