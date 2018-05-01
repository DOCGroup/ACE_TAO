// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_MT_Dispatching.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CEC_MT_DISPATCHING_H
#define TAO_CEC_MT_DISPATCHING_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEvent/CEC_Dispatching.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEvent/CEC_Dispatching_Task.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_CEC_EventChannel;

/**
 * @class TAO_CEC_MT_Dispatching
 *
 * @brief Dispatching strategy that minimizes mt inversion.
 *
 * This strategy uses a single queue, serviced by one or more
 * threads.  It's main purpose is to decouple the suppliers from
 * the client execution time, specially in the collocated case.
 */
class TAO_Event_Serv_Export TAO_CEC_MT_Dispatching : public TAO_CEC_Dispatching
{
public:
  /// Constructor
  /// It will create @a nthreads servicing threads...
  TAO_CEC_MT_Dispatching (int nthreads,
                         int thread_creation_flags,
                         int thread_priority,
                         int force_activate);

  // = The EC_Dispatching methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual void push (TAO_CEC_ProxyPushSupplier* proxy,
                     const CORBA::Any & event);
  virtual void push_nocopy (TAO_CEC_ProxyPushSupplier* proxy,
                            CORBA::Any& event);
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  virtual void invoke (TAO_CEC_ProxyPushSupplier *proxy,
                       const TAO_CEC_TypedEvent & typed_event);
  virtual void invoke_nocopy (TAO_CEC_ProxyPushSupplier *proxy,
                              TAO_CEC_TypedEvent & typed_event);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

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
  TAO_CEC_Dispatching_Task task_;

  /// Synchronize access to internal data
  TAO_SYNCH_MUTEX lock_;

  /// Are the threads running?
  int active_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_CEC_MT_DISPATCHING_H */
