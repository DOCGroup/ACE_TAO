/* -*- C++ -*- */

//=============================================================================
/**
 *  @file RT_Task.h
 *
 *  $Id$
 *
 *  @author Tim Harrison (harrison@cs.wustl.edu)
 *
 *  Wrapper on top of ACE Task that integrates an Active Object with
 *  the Event Service.
 *
 *
 */
//=============================================================================


#ifndef ACE_RT_TASK_H
#define ACE_RT_TASK_H
#include "ace/pre.h"

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecSchedulerC.h"
#include "old_event_export.h"

/**
 * @class ACE_RT_Thread_Manager
 *
 *
 */
class TAO_RTOLDEvent_Export ACE_RT_Thread_Manager : public ACE_Thread_Manager
{
public:
  /// Default construction.
  ACE_RT_Thread_Manager (void);

  /// Any threads spawned will be suspended until <unsuspend_spawns> is
  /// called.
  void suspend_spawns (void);

  /// Resumes all threads.
  void unsuspend_spawns (void);

protected:
  virtual int spawn_i (ACE_THR_FUNC func,
                       void *args,
                       long flags,
                       ACE_thread_t * = 0,
                       ACE_hthread_t *t_handle = 0,
                       long priority = 0,
                       int grp_id = -1,
                       void *stack = 0,
                       size_t stack_size = 0,
                       ACE_Task_Base *task = 0);

  int flags_;
};

// ************************************************************

/**
 * @class ACE_RT_Task_Command
 *
 *
 */
class TAO_RTOLDEvent_Export ACE_RT_Task_Command : public ACE_Message_Block
{
public:
  /// Guarantees that derived destructors get called.
  virtual ~ACE_RT_Task_Command (void) {}

  enum { RELEASE, UNGETQ };

  /**
   * Execute the command.  Returning 1 will make the calling thread
   * exit.  Returning 0 will allow the thread to continue dispatching
   * commands.  If <command_action> returns as RELEASE, the command
   * will be released.  If <command_action> == UNGETQ, then the
   * command will be requeued and dispatched again.
   */
  virtual int execute (u_long &command_action) = 0;
};

// ************************************************************

typedef ACE_Task<ACE_SYNCH> ACE_ES_TASK;
typedef ACE_Message_Queue<ACE_SYNCH> ACE_ES_QUEUE;

/**
 * @class ACE_RT_Task
 *
 * @brief ACE Real-Time Task
 *
 * Real-Time Active Object that integrates with a global
 * scheduler and Event Service.  For now, none of the management
 * methods are synchronized.  If it turns out that multiple
 * threads will be calling the management methods, then we can
 * add synchronization.  For the most part, RT_Task threads
 * should be dequeueing commands from the message queue.  Only
 * one thread should be calling any management methods.
 */
class TAO_RTOLDEvent_Export ACE_RT_Task : public ACE_ES_TASK
{
  friend class ACE_RT_Thread_Manager;
public:
  /// Default construction.
  ACE_RT_Task (RtecScheduler::Scheduler_ptr scheduler);

  /// Deactivates the queue.
  ~ACE_RT_Task (void);

  // = Management methods.

  /**
   * <name> is used to look up our qos info from the scheduler.  If
   * <name> == 0, then we create a "unique" name and ask the scheduler
   * for a new qos structure.  If we find an existing qos structure,
   * calls this->synch_threads and returns 1.  If a qos structure is not
   * found, but created returns 0 and does not call synch_threads.
   * Returns -1 on failure.
   */
  int open_task (const char* name = 0);

  /// Enqueue a request.  Returns 0 on success, -1 on failure.  If the
  /// task is shutdown, -1 is returned with errno == EPIPE.
  int try_put (ACE_Message_Block *mb);

  /**
   * If active, shutdown all running thread.  Since this is
   * accomplished via queued shutdown messages, this has the effect of
   * flushing the queue.  Once all threads exit, threads_closed will
   * be called.  If this is a passive object, then the queue will be
   * flushed and threads_closed will be called.
   */
  int shutdown_task (void);

  /**
   * Compare <threads> with what is actually running.  If there are
   * any differences, update this RT_Task.  This may involve spawning
   * more threads or changing thread priorities, etc.  This can be
   * used to close all threads by sending a 0.
   */
  int synch_threads (size_t threads);

  /// QOS accessor.  The behavior of the task can be changed by setting
  /// this and then calling this->synch_threads.
  RtecScheduler::handle_t rt_info (void);

  /**
   * Called when every thread has exited.  This hook allows
   * applications to specify semantics when all threads have exited.
   * For instance, the Dispatching Module uses this hook to delete
   * itself when an application is shutting down.
   */
  virtual void threads_closed (void);

  /**
   * This is called the first time the thread is spawned.  <priority>
   * is the priority of the current thread.  If this returns != 1
   * (e.g., 0), the event loop will execute (calling this->svc_one).
   * If this returns 1, the event loop will not execute.
   */
  virtual int svc_hook (RtecScheduler::OS_Priority priority);

  /// Call this->getq once and execute the command.  Returns the result
  /// of command->execute ().
  virtual int svc_one (void);

protected:
  /// Run by each thread spawned.  Each thread dequeues
  /// ACE_RT_Task_Commands and executes them.
  virtual int svc (void);

  /// Called each time a thread exits.
  virtual int close (u_long flags = 0);

  /// Enqueues shutdown message for every thread in the task.
  void close_all_threads (void);

protected:
  /// Scheduling characteristics of this active object.
  RtecScheduler::handle_t rt_info_;

  /// Set to 1 when this->shutdown_threads or this->close_queue is
  /// called.  Keeps us from enqueuing more that one shutdown message.
  int closed_;

  /// The scheduler.
  RtecScheduler::Scheduler_var scheduler_;
};

#if defined (__ACE_INLINE__)
#include "RT_Task.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RT_TASK_H */
