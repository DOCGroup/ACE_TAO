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
#include "ace/Message_Queue.h"
#include "ace/Thread_Manager.h"
#include "ace/Synch_T.h"

// Forward decls...
template <ACE_SYNCH_1> class ACE_Module;

class ACE_Task_Flags
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

class ACE_Task_Base : public ACE_Service_Object
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
  virtual int open (void *args = 0) = 0;
  // Hook called to open a Task.  <args> can be used to pass arbitrary
  // information into <open>.

  virtual int close (u_long flags = 0) = 0;
  // Hook called to close a Task.

  // = Immediate and deferred processing methods, respectively.
  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0) = 0;
  // Transfer msg into the queue to handle immediate processing. 

  virtual int svc (void);
  // Run by a daemon thread to handle deferred processing.

  // = Active object activation method.
  virtual int activate (long flags = THR_NEW_LWP, 
			int n_threads = 1, 
			int force_active = 0,
			u_int priority = 0,
			int grp_id = -1,
			ACE_Task_Base *task = NULL); 
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

  int grp_id (void);
  // Get the current group id.

  void grp_id (int);
  // Set the current group id.

  ACE_Thread_Manager *thr_mgr (void);
  // Gets the thread manager associated with this Task.

  void thr_mgr (ACE_Thread_Manager *);
  // Set the thread manager associated with this Task.

  size_t thr_count (void);
  // Returns the number of threads currently running within a task.
  // If we're a passive object this value is 0, else it's > 0.

  static void *svc_run (ACE_Task_Base *); 
  // Routine that runs the service routine as a daemon thread. 

  int is_reader (void);	
  // True if queue is a reader, else false.

  int is_writer (void);	
  // True if queue is a writer, else false.

  void thr_count_dec (void);
  // Atomically decrement the thread count by 1.  This should only be
  // called by the <ACE_Task_Exit> class destructor.

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
  // This maintains the group id of the 

#if defined (ACE_MT_SAFE)
  ACE_Thread_Mutex lock_;
  // Protect the state of a Task during concurrent operations, but
  // only if we're configured as MT safe...
#endif /* ACE_MT_SAFE */
};

template <ACE_SYNCH_1>
class ACE_Task : public ACE_Task_Base
  // = TITLE
  //    Primary interface for application message processing, as well 
  //    as input and output message queueing.
  //
  // = DESCRIPTION
  //    This class serves as the basis for passive and active objects
  //    in ACE.  
{
friend class ACE_Module<ACE_SYNCH_2>;
friend class ACE_Module_Type;
public:
  // = Initialization/termination methods.
  ACE_Task (ACE_Thread_Manager *thr_mgr = 0, 
	    ACE_Message_Queue<ACE_SYNCH_2> *mq = 0);
  // Initialize a Task, supplying a thread manager and a message
  // queue.  If the user doesn't supply a ACE_Message_Queue pointer
  // then we'll allocate one dynamically.  Otherwise, we'll use the
  // one they give.

  virtual ~ACE_Task (void);	
  // Destructor.

  ACE_Message_Queue<ACE_SYNCH_2> *msg_queue (void);
  // Gets the message queue associated with this task.

  void msg_queue (ACE_Message_Queue<ACE_SYNCH_2> *);
  // Sets the message queue associated with this task.

public: // Should be protected: 
  // = Message queue manipulation methods.

  int putq (ACE_Message_Block *, ACE_Time_Value *tv = 0);    
  // Insert message into the message list.

  int getq (ACE_Message_Block *&mb, ACE_Time_Value *tv = 0);
  // Extract the first message from the list (blocking).

  int ungetq (ACE_Message_Block *, ACE_Time_Value *tv = 0);	 
  // Return a message to the queue.

  int can_put (ACE_Message_Block *); 
  // Tests whether we can enqueue a message without blocking.

  int reply (ACE_Message_Block *, ACE_Time_Value *tv = 0);	     
  // Turn the message back around.

  int put_next (ACE_Message_Block *msg, ACE_Time_Value *tv = 0);
  // Transfer message to the adjacent ACE_Task in a ACE_Stream.

  // = ACE_Task utility routines to identify names et al.
  const char *name (void) const;
  // Return the name of the enclosing Module if there's one associated
  // with the Task, else returns 0.

  // = Pointers to next ACE_Task_Base (if ACE is part of an ACE_Stream).
  ACE_Task<ACE_SYNCH_2> *next (void);
  // Get next Task pointer.
  void next (ACE_Task<ACE_SYNCH_2> *);
  // Set next Task pointer.

  ACE_Task<ACE_SYNCH_2> *sibling (void);
  // Return the Task's sibling if there's one associated with the
  // Task's Module, else returns 0.

  ACE_Module<ACE_SYNCH_2> *module (void) const;
  // Return the Task's Module if there is one, else returns 0.

  int flush (u_long flag = ACE_Task_Flags::ACE_FLUSHALL); /* Flush the queue */
  // Special routines corresponding to certain message types.

  void water_marks (ACE_IO_Cntl_Msg::ACE_IO_Cntl_Cmds, size_t);
  // Manipulate watermarks.

  ACE_Message_Queue<ACE_SYNCH_2> *msg_queue_;	
  // List of messages on the ACE_Task..

  int delete_msg_queue_;
  // 1 if should delete Message_Queue, 0 otherwise.

  ACE_Module<ACE_SYNCH_2> *mod_;		
  // Back-pointer to the enclosing module.

  ACE_Task<ACE_SYNCH_2> *next_;
  // Pointer to adjacent ACE_Task.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Task_Exit
  // = TITLE
  //    Keep exit information for a Task in thread specific storage so
  //    that the Task::close() method will get called no matter how
  //    the thread exits (e.g., via Thread::exit() or by "falling off
  //    the end of Task::svc_run").
  //
  // = DESCRIPTION
  //    This clever little helper class is stored in thread-specific
  //    storage using the ACE_TSS wrapper.  When a thread
  //    exits the ACE_TSS::cleanup() function calls
  //    "delete" on this object, thereby closing it down gracefully.
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

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Task.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Task.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TASK_H */
