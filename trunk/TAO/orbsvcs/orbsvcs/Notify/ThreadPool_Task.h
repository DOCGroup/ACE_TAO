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
#include "Worker_Task.h"
#include "Types.h"

class TAO_NS_Buffering_Strategy;

/**
 * @class TAO_NS_ThreadPool_Task
 *
 * @brief Implements a Thread Pool Worker Task.
 *
 */
class TAO_Notify_Export TAO_NS_ThreadPool_Task : public TAO_NS_Worker_Task, ACE_Task<ACE_SYNCH>
{
  friend class TAO_NS_Method_Request_Shutdown;

public:
  /// Constuctor
  TAO_NS_ThreadPool_Task (void);

  /// Destructor
  ~TAO_NS_ThreadPool_Task ();

  /// Activate the threadpool
  void init (const NotifyExt::ThreadPoolParams& tp_params, TAO_NS_AdminProperties& admin_properties ACE_ENV_ARG_DECL);

  /// Queue the request
  virtual void exec (TAO_NS_Method_Request& method_request);

  /// Shutdown task
  virtual void shutdown (void);

  /// Update QoS Properties.
  virtual void update_qos_properties (const TAO_NS_QoSProperties& qos_properties);

protected:
    /// task svc
  virtual int svc (void);

private:
  /// Message Queue
  ACE_Message_Queue<ACE_SYNCH>& msg_queue_;

  /// The buffering strategy to use.
  TAO_NS_Buffering_Strategy* buffering_strategy_;

  /// We need to decrement the event_count_ everytime we dequeue a command
  /// object.
  TAO_NS_Signal_Property_Long* queue_length_;
};

#if defined (__ACE_INLINE__)
#include "ThreadPool_Task.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_THREADPOOL_TASK_H */
