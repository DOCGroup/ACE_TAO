// -*- C++ -*-

/**
 *  @file ThreadPool_Task.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_THREADPOOL_TASK_H
#define TAO_Notify_THREADPOOL_TASK_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#include "orbsvcs/Notify/Timer_Queue.h"
#include "orbsvcs/Notify/AdminProperties.h"
#include "orbsvcs/Notify/Worker_Task.h"

#include "ace/Task.h"
#include "ace/Message_Queue.h"
#include "ace/Reactor.h"
#include "ace/Null_Condition.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_Buffering_Strategy;

/**
 * @class TAO_Notify_ThreadPool_Task
 *
 * @brief Implements a Thread Pool Worker Task.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_ThreadPool_Task
  : public TAO_Notify_Worker_Task
  , public ACE_Task<ACE_NULL_SYNCH>
{
  friend class TAO_Notify_Method_Request_Shutdown;

public:
  /// Constructor
  TAO_Notify_ThreadPool_Task (void);

  /// Destructor
  virtual ~TAO_Notify_ThreadPool_Task ();

  /// Call the base class init
  virtual int init (int argc, ACE_TCHAR **argv);

  /// release reference to my self.
  virtual int close (u_long flags);

  /// Activate the threadpool
  void init (const NotifyExt::ThreadPoolParams& tp_params, const TAO_Notify_AdminProperties::Ptr& admin_properties);

  /// Queue the request
  virtual void execute (TAO_Notify_Method_Request& method_request);

  /// Shutdown task
  virtual void shutdown ();

  /// Update QoS Properties.
  virtual void update_qos_properties (const TAO_Notify_QoSProperties& qos_properties);

  /// The object used by clients to register timers.
  virtual TAO_Notify_Timer* timer (void);

  /// Provide access to the underlying buffering strategy
  TAO_Notify_Buffering_Strategy* buffering_strategy (void);

protected:
  /// Task svc
  virtual int svc (void);

private:
  /// Release
  virtual void release (void);

  /// The buffering strategy to use.
  ACE_Auto_Ptr< TAO_Notify_Buffering_Strategy > buffering_strategy_;

  /// Shutdown
  bool shutdown_;

  /// The Queue based timer.
  TAO_Notify_Timer_Queue::Ptr timer_;
};


TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_Notify_THREADPOOL_TASK_H */
