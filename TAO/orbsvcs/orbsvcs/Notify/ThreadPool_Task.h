/* -*- C++ -*- */
/**
 *  @file ThreadPool_Task.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_THREADPOOL_TASK_H
#define TAO_NS_THREADPOOL_TASK_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Message_Queue.h"
#include "ace/Reactor.h"
#include "AdminProperties.h"
#include "Worker_Task.h"

class TAO_NS_Buffering_Strategy;
class TAO_NS_Timer_Queue;

/**
 * @class TAO_NS_ThreadPool_Task
 *
 * @brief Implements a Thread Pool Worker Task.
 *
 */
class TAO_Notify_Export TAO_NS_ThreadPool_Task : public TAO_NS_Worker_Task, ACE_Task<ACE_NULL_SYNCH>
{
  friend class TAO_NS_Method_Request_Shutdown;

public:
  /// Constuctor
  TAO_NS_ThreadPool_Task (void);

  /// Destructor
  ~TAO_NS_ThreadPool_Task ();

  /// Call the base class init
  virtual int init (int argc, char **argv);

  virtual int close (u_long flags);

  /// Release
  virtual void release (void);

  /// Activate the threadpool
  void init (const NotifyExt::ThreadPoolParams& tp_params, TAO_NS_AdminProperties_var& admin_properties ACE_ENV_ARG_DECL);

  /// Queue the request
  virtual void exec (TAO_NS_Method_Request& method_request);

  /// Shutdown task
  virtual void shutdown (void);

  /// Update QoS Properties.
  virtual void update_qos_properties (const TAO_NS_QoSProperties& qos_properties);

  /// The object used by clients to register timers.
  virtual TAO_NS_Timer* timer (void);

  /// Access the Buffering Strategy.
  TAO_NS_Buffering_Strategy* buffering_strategy (void);

protected:
    /// task svc
  virtual int svc (void);

private:
  /// The buffering strategy to use.
  TAO_NS_Buffering_Strategy* buffering_strategy_;

  /// Shutdown
  int shutdown_;

  /// The Queue based timer.
  TAO_NS_Timer_Queue* timer_;
};

#if defined (__ACE_INLINE__)
#include "ThreadPool_Task.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_THREADPOOL_TASK_H */
