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
#include "ace/Activation_Queue.h"
#include "orbsvcs/NotifyExtC.h"
#include "Worker_Task.h"

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
  void init (NotifyExt::ThreadPoolParams* tp_params ACE_ENV_ARG_DECL);

  /// Queue the request
  virtual void exec (TAO_NS_Method_Request& method_request);

  /// Shutdown task
  virtual void shutdown (void);

protected:
    /// task svc
  virtual int svc (void);

private:
  /// Activation Queue
  ACE_Activation_Queue activation_queue_;
};

#if defined (__ACE_INLINE__)
#include "ThreadPool_Task.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_THREADPOOL_TASK_H */
