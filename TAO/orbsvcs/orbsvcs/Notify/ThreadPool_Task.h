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

#ifndef TAO_Notify_THREADPOOL_TASK_H
#define TAO_Notify_THREADPOOL_TASK_H
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

class TAO_Notify_Buffering_Strategy;
class TAO_Notify_Timer_Queue;

/**
 * @class TAO_Notify_ThreadPool_Task
 *
 * @brief Implements a Thread Pool Worker Task.
 *
 */
class TAO_Notify_Export TAO_Notify_ThreadPool_Task : public TAO_Notify_Worker_Task, ACE_Task<ACE_NULL_SYNCH>
{
  friend class TAO_Notify_Method_Request_Shutdown;

public:
  /// Constuctor
  TAO_Notify_ThreadPool_Task (void);

  /// Destructor
  ~TAO_Notify_ThreadPool_Task ();

  /// Call the base class init
  virtual int init (int argc, char **argv);

  virtual int close (u_long flags);

  /// Release
  virtual void release (void);

  /// Activate the threadpool
  void init (const NotifyExt::ThreadPoolParams& tp_params, TAO_Notify_AdminProperties_var& admin_properties ACE_ENV_ARG_DECL);

  /// Queue the request
  virtual void execute (TAO_Notify_Method_Request_No_Copy& method_request ACE_ENV_ARG_DECL);

  /// Shutdown task
  virtual void shutdown (void);

  /// Update QoS Properties.
  virtual void update_qos_properties (const TAO_Notify_QoSProperties& qos_properties);

  /// The object used by clients to register timers.
  virtual TAO_Notify_Timer* timer (void);

  /// Access the Buffering Strategy.
  TAO_Notify_Buffering_Strategy* buffering_strategy (void);

protected:
    /// task svc
  virtual int svc (void);

private:
  /// The buffering strategy to use.
  TAO_Notify_Buffering_Strategy* buffering_strategy_;

  /// Shutdown
  int shutdown_;

  /// The Queue based timer.
  TAO_Notify_Timer_Queue* timer_;
};

#if defined (__ACE_INLINE__)
#include "ThreadPool_Task.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_THREADPOOL_TASK_H */
