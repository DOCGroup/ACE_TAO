/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Task.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#if !defined (ACE_TASK_H)
#define ACE_TASK_H

#include "ace/Service_Object.h"
#include "ace/Thread_Manager.h"

class ACE_Export ACE_Task_Flags
{
public:
  // = TITLE
  //    These flags are used within the ACE_Task.
  //
  // = DESCRIPTION
  //    These flags should be hidden within ACE_Task.  Unfortunately, the
  //    HP/UX C++ compiler can't grok this...  Fortunately, there's no
  //    code defined here, so we don't have to worry about multiple
  //    definitions.
  enum
  {
    ACE_READER     = 01,  // Identifies a Task as being the "reader" in a Module.
    ACE_FLUSHDATA  = 02,  // Just flush data messages in the queue.
    ACE_FLUSHALL   = 04,  // Flush all messages in the Queue.
    ACE_FLUSHR     = 010, // flush read queue
    ACE_FLUSHW     = 020, // flush write queue
    ACE_FLUSHRW    = 030  // flush both queues
  };
};

class ACE_Export ACE_Task_Base : public ACE_Service_Object
{
  // = TITLE
  //    Direct base class for the ACE_Task template.
  //
  // = DESCRIPTION
  //    This class factors out the non-template code in order to
  //    reduce template bloat, as well as to make it possible for the
  //    <ACE_Thread_Manager> to store <ACE_Task_Base> *'s
  //    polymorphically.
public:
  // = Initialization and termination methods.
  ACE_Task_Base (ACE_Thread_Manager *);
  // Constructor.

  virtual ~ACE_Task_Base (void);
  // Destructor.

  // = Initialization and termination hooks 

  // Note that these methods *must* be defined by subclasses.

  virtual int open (void *args = 0);
  // Hook called to open a Task.  <args> can be used to pass arbitrary
  // information into <open>.

  virtual int close (u_long flags = 0);
  // Hook called from <ACE_Task_Exit> when during thread exit and from
  // the default implemenation of <module_closed>.  In general, this
  // method shouldn't be called directly by an application,
  // particularly if the <Task> is running as an Active Object.
  // Instead, a special message should be passed into the <Task> via
  // the <put> method defined below, and the <svc> method should
  // interpret this as a flag to shut down the <Task>.

  virtual int module_closed (void);
  // Hook called during ACE_Module::close().  The default
  // implementation calls forwards the call to close(1).  Please
  // notice the changed value of the default argument of close().
  // This allows tasks to differ between the call has been originated
  // from <ACE_Task_Exit> or from <module_closed>.  Be aware that
  // close(0) will be also called when a thread associated with the
  // ACE_Task instance exits.

  // = Immediate and deferred processing methods, respectively.
  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0);
  // Transfer msg into the queue to handle immediate processing.

  virtual int svc (void);
  // Run by a daemon thread to handle deferred processing.

  // = Active object activation method.
  virtual int activate (long flags = THR_NEW_LWP,
			int n_threads = 1,
			int force_active = 0,
			long priority = ACE_DEFAULT_THREAD_PRIORITY,
			int grp_id = -1,
			ACE_Task_Base *task = 0,
			ACE_hthread_t thread_handles[] = 0,
                        void *stack[] = 0,
                        size_t stack_size[] = 0);
  // Turn the task into an active object, i.e., having <n_threads> of
  // control, all running at the <priority> level (see below) with the same
  // <grp_id>, all of which invoke <Task::svc>.  Returns -1 if failure
  // occurs, returns 1 if Task is already an active object and
  // <force_active> is false (doesn't *not* create a new thread in
  // this case), and returns 0 if Task was not already an active
  // object and a thread is created successfully or thread is an
  // active object and <force_active> is true.
  //
  // The <{flags}> are a bitwise-OR of the following:
  // = BEGIN<INDENT>
  // THR_CANCEL_DISABLE, THR_CANCEL_ENABLE, THR_CANCEL_DEFERRED,
  // THR_CANCEL_ASYNCHRONOUS, THR_BOUND, THR_NEW_LWP, THR_DETACHED,
  // THR_SUSPENDED, THR_DAEMON, THR_JOINABLE, THR_SCHED_FIFO,
  // THR_SCHED_RR, THR_SCHED_DEFAULT
  // = END<INDENT>
  //
  // By default, or if <{priority}> is set to
  // ACE_DEFAULT_THREAD_PRIORITY, an "appropriate" priority value for
  // the given scheduling policy (specified in <{flags}>, e.g.,
  // <THR_SCHED_DEFAULT>) is used.  This value is calculated
  // dynamically, and is the median value between the minimum and
  // maximum priority values for the given policy.  If an explicit
  // value is given, it is used.  Note that actual priority values are
  // EXTREMEMLY implementation-dependent, and are probably best
  // avoided.
  //
  // If <thread_handles> != 0 it is assumed to be an array of <n>
  // thread_handles that will be assigned the values of the thread
  // handles being spawned.  Returns -1 on failure (<errno> will
  // explain...), otherwise returns the group id of the threads.
  //
  // If <stack> != 0 it is assumed to be an array of <n> pointers to
  // the base of the stacks to use for the threads being spawned.
  // Likewise, if <stack_size> != 0 it is assumed to be an array of
  // <n> values indicating how big each of the corresponding <stack>s
  // are.

  virtual int wait (void);
  // Wait for all threads running in this task to exit.

  // = Suspend/resume a Task
  virtual int suspend (void);
  // Suspend a task.
  virtual int resume (void);
  // Resume a suspended task.

  int grp_id (void) const;
  // Get the current group id.

  void grp_id (int);
  // Set the current group id.

  ACE_Thread_Manager *thr_mgr (void) const;
  // Gets the thread manager associated with this Task.

  void thr_mgr (ACE_Thread_Manager *);
  // Set the thread manager associated with this Task.

  int is_reader (void) const;
  // True if queue is a reader, else false.

  int is_writer (void) const;
  // True if queue is a writer, else false.

  size_t thr_count (void) const;
  // Returns the number of threads currently running within a task.
  // If we're a passive object this value is 0, else it's > 0.

  void thr_count_dec (void);
  // Atomically decrement the thread count by 1.  This should only be
  // called by the <ACE_Task_Exit> class destructor.

  static void *svc_run (void *);
  // Routine that runs the service routine as a daemon thread.

  static void cleanup (void *object, void *params);
  // Cleanup hook that is called when a thread exits to gracefully
  // shutdown an <ACE_Task>.

  // = Internal data (should be private...).
// private:

  size_t thr_count_;
  // Count of the number of threads running within the task.  If this
  // value is > 0 then we're an active object and the value of
  // <thr_count_> is the number of active threads at this instant.  If
  // the value == 0, then we're a passive object.

  ACE_Thread_Manager *thr_mgr_;
  // Multi-threading manager.

  u_long flags_;
  // ACE_Task flags.

  int grp_id_;
  // This maintains the group id of the Task.

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  ACE_Thread_Mutex lock_;
  // Protect the state of a Task during concurrent operations, but
  // only if we're configured as MT safe...
#endif /* ACE_MT_SAFE */

private:

  // = Disallow these operations.
  ACE_Task_Base &operator= (const ACE_Task_Base &);
  ACE_Task_Base (const ACE_Task_Base &);
};

#if defined (__ACE_INLINE__)
#include "ace/Task.i"
#endif /* __ACE_INLINE__ */

// Include the ACE_Task templates classes at this point.
#include "ace/Task_T.h"

#endif /* ACE_TASK_H */
