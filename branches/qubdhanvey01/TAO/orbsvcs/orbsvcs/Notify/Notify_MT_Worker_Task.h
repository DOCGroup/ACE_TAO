/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_MT_Worker_Task.h
 *
 *  $Id$
 *
 * MT Worker task
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_MT_NOTIFY_WORKER_TASK_H
#define TAO_MT_NOTIFY_WORKER_TASK_H

#include "ace/pre.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_Worker_Task.h"
#include "Notify_AdminProperties.h"

class TAO_Notify_Event_Processor;
class TAO_Notify_Buffering_Strategy;
class TAO_Notify_QoSAdmin_i;

/**
 * @class TAO_Notify_MT_Worker_Task
 *
 * @brief TAO_Notify_MT_Worker_Task
 *
 * A MT worker task that we use for various event processing jobs.
 * Also see TAO_Notify_Command - This task executes Notify_Command objects.
 */
class TAO_Notify_Export TAO_Notify_MT_Worker_Task : public TAO_Notify_Worker_Task, public ACE_Task<ACE_SYNCH>
{
public:
  // = Initialization and termination code
  /// Constructor.
  TAO_Notify_MT_Worker_Task (int n_threads = 1,
                             long flags = THR_NEW_LWP | THR_JOINABLE,
                             int force_active = 0,
                             long priority = ACE_DEFAULT_THREAD_PRIORITY);

  /// Destructor.
  ~TAO_Notify_MT_Worker_Task ();

  /// Init the task
  virtual int init_task (TAO_Notify_AdminProperties* const admin_properties,
                         TAO_Notify_QoSAdmin_i* const qos_properties);

  /// Shutdown this task.
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// Process the command.
  virtual int process_event (TAO_Notify_Command *mb ACE_ENV_ARG_DECL, ACE_Time_Value *tv = 0);

  virtual void update_admin (TAO_Notify_AdminProperties& admin);
  virtual void update_qos (TAO_Notify_QoSAdmin_i& qos_admin);

 protected:
  /// svc command processes objects stored in the message queue.
  virtual int svc (void);

  /// Close hook.
  virtual int close (u_long);

  /// The buffering strategy to use.
  TAO_Notify_Buffering_Strategy* buffering_strategy_;

  /// We need to decrement the event_count_ everytime we dequeue a command
  /// object.
  TAO_Notify_Signal_Property_Long* queue_length_;

  // = Parameters to activate
  int n_threads_;
  long flags_;
  int force_active_;
  long priority_;
};

//****************************************************************************************

 /**
  * @class TAO_Notify_Shutdown_Command
  *
  * @brief TAO_Notify_Shutdown_Command
  *
  * Shutdown command to shutdown the task.
  */
class TAO_Notify_Export TAO_Notify_Shutdown_Command : public TAO_Notify_Command
{
 public:

  TAO_Notify_Shutdown_Command (void);

  /// Returns -1. This signals worker threads to finish servicing requests.
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);
};

//****************************************************************************************

#include "ace/post.h"
#endif /* TAO_MT_NOTIFY_WORKER_TASK_H */
