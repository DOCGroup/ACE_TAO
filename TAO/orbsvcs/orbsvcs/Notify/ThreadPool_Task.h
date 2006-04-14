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

#include /**/ "ace/pre.h"

#include "notify_serv_export.h"

#include "Timer_Queue.h"
#include "AdminProperties.h"
#include "Worker_Task.h"

#include "ace/Task.h"
#include "ace/Message_Queue.h"
#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_Buffering_Strategy;

/**
 * @class TAO_Notify_ThreadPool_Task
 *
 * @brief Implements a Thread Pool Worker Task.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_ThreadPool_Task : public TAO_Notify_Worker_Task, public ACE_Task<ACE_NULL_SYNCH>
{
  friend class TAO_Notify_Method_Request_Shutdown;

public:
  /// Constuctor
  TAO_Notify_ThreadPool_Task (void);

  /// Destructor
  virtual ~TAO_Notify_ThreadPool_Task ();

  /// Call the base class init
  virtual int init (int argc, ACE_TCHAR **argv);

  /// release reference to my self.
  virtual int close (u_long flags);

  /// Activate the threadpool
  void init (const NotifyExt::ThreadPoolParams& tp_params, TAO_Notify_AdminProperties::Ptr& admin_properties ACE_ENV_ARG_DECL);

  /// Queue the request
  virtual void execute (TAO_Notify_Method_Request& method_request ACE_ENV_ARG_DECL);

  /// Shutdown task
  virtual void shutdown ();

  /// Update QoS Properties.
  virtual void update_qos_properties (const TAO_Notify_QoSProperties& qos_properties);

  /// The object used by clients to register timers.
  virtual TAO_Notify_Timer* timer (void);

protected:
  /// Task svc
  virtual int svc (void);

private:
  /// Release
  virtual void release (void);

  /// wait for all threads to exit svc()
  virtual void wait_for_shutdown ();

  /// The buffering strategy to use.
  ACE_Auto_Ptr< TAO_Notify_Buffering_Strategy > buffering_strategy_;

  /// Shutdown
  bool shutdown_;

  /// The Queue based timer.
  TAO_Notify_Timer_Queue::Ptr timer_;

  // Since this class already inherited from ACE_Event_Handler
  // I did not want to conflict with a possible parent
  /// implementation of handle_exception.
  class Shutdown_Handler : public ACE_Event_Handler
  {
  public:
    Shutdown_Handler (TAO_Notify_ThreadPool_Task* owner) : owner_ (owner) {};

    /// wait for all threads to complete in another thread
    virtual int handle_exception (ACE_HANDLE fd = ACE_INVALID_HANDLE)
    {
      ACE_UNUSED_ARG (fd);
      owner_->wait_for_shutdown ();
      return 0;
    }
  private:
    TAO_Notify_ThreadPool_Task* owner_;
  };

  friend class Shutdown_Handler;

  Shutdown_Handler shutdown_handler_;

};

#include /**/ "ace/post.h"

#endif /* TAO_Notify_THREADPOOL_TASK_H */
