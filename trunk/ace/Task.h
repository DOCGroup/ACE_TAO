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
  // = TITLE
  //    These flags are used within the ACE_Task.
  // 
  // = DESCRIPTION
  //    These flags should be hidden within ACE_Task.  Unfortunately, the
  //    HP/UX C++ compiler can't grok this...  Fortunately, there's no
  //    code defined here, so we don't have to worry about multiple
  //    definitions. 
{
public:
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
  // = TITLE
  //    Direct base class for the ACE_Task template.
  //
  // = DESCRIPTION
  //    This class factors out the non-template code in order to
  //    reduce template bloat, as well as to make it possible for the
  //    <ACE_Thread_Manager> to store <ACE_Task_Base> *'s
  //    polymorphically.
{
public:
  // = Initialization method.
  ACE_Task_Base (ACE_Thread_Manager *);

  // = Initialization and termination hooks (note that these *must* be defined by subclasses).
  virtual int open (void *args = 0);
  // Hook called to open a Task.  <args> can be used to pass arbitrary
  // information into <open>.

  virtual int close (u_long flags = 0);
  // Hook called from ACE_Task_Exit when during thread exit and from
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
  // from ACE_Task_Exit or from module_closed().  Be aware that
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
			long priority = -1,
			int grp_id = -1,
			ACE_Task_Base *task = 0); 
  // Turn the task into an active object, i.e., having <n_threads> of
  // control, all running at the <priority> level with the same
  // <grp_id>, all of which invoke <Task::svc>.  Returns -1 if failure
  // occurs, returns 1 if Task is already an active object and
  // <force_active> is false (doesn't *not* create a new thread in
  // this case), and returns 0 if Task was not already an active
  // object and a thread is created successfully or thread is an
  // active object and <force_active> is true.

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

  // Routine that runs the service routine as a daemon thread.
  static void *svc_run (void *);

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

#if defined (ACE_MT_SAFE)
  ACE_Thread_Mutex lock_;
  // Protect the state of a Task during concurrent operations, but
  // only if we're configured as MT safe...
#endif /* ACE_MT_SAFE */
};

class ACE_Export ACE_Task_Exit
  // = TITLE
  //    Keep exit information for a Task in thread specific storage so
  //    that the Task::close() method will get called no matter how
  //    the thread exits (e.g., via <Thread::exit>, C++ or Win32
  //    exception, "falling off the end" of <Task::svc_run>, etc.).
  //
  // = DESCRIPTION
  //    This clever little helper class is stored in thread-specific
  //    storage using the <ACE_TSS> wrapper.  When a thread exits the
  //    <ACE_TSS::cleanup> function deletes this object, thereby
  //    closing it down gracefully.
{
public:
  ACE_Task_Exit (void);
  // Capture the Task object that will be cleaned up automatically.

  void set_task (ACE_Task_Base *t);
  // Set the this pointer...

  ACE_Task_Base *get_task (void);
  // Get the pointer to the ACE_Task.

  void *status (void *s);
  // Set the exit status.

  void *status (void);
  // Get the exit status.

  ~ACE_Task_Exit (void);
  // Destructor calls the <close> method of the captured Task on exit.

  static ACE_Task_Exit *instance (void);
  // Singleton access point.

private:
  ACE_Task_Base *t_;
  // Pointer to the captured Task.

  void *status_;
  // Exit status...

  ACE_Thread_Control tc_;
  // This is used to make sure that an ACE_Task registers and
  // deregisters with the ACE_Thread_Manager correctly.

#if defined (ACE_MT_SAFE) && !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static ACE_Thread_Mutex ace_task_lock_;
  // Lock the creation of the Singleton.
#endif /* defined (ACE_MT_SAFE) && !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */
};

#if defined (__ACE_INLINE__)
#include "ace/Task.i"
#endif /* __ACE_INLINE__ */

// Include the ACE_Task templates classes at this point.
#include "ace/Task_T.h"

#endif /* ACE_TASK_H */
