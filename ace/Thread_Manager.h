/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Thread_Manager.h 
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_THREAD_MANAGER_H)
#define ACE_THREAD_MANAGER_H

#include "ace/Thread.h"
#include "ace/Synch.h"

// Forward declarations.
class ACE_Task_Base;
class ACE_Thread_Manager;

class ACE_Export ACE_Thread_Descriptor
  // = Title
  //    Information for controlling threads that run under the control
  //    of the <Thread_Manager>.
{
friend class ACE_Thread_Manager;
public:
  // = Initialization method.
  ACE_Thread_Descriptor (void);

  // = Accessor methods.
  ACE_thread_t self (void);
  // Unique thread id.

  void self (ACE_hthread_t &);
  // Unique handle to thread (used by Win32 and AIX).

  int grp_id (void);
  // Group ID.

  ACE_Thread_State state (void);
  // Current state of the thread.

  ACE_Task_Base *task_;
  // Pointer to an ACE_Task;

  void dump (void) const;
  // Dump the state of an object.

private:

  ACE_thread_t thr_id_;
  // Unique thread ID.
    
  ACE_hthread_t thr_handle_;
  // Unique handle to thread (used by Win32 and AIX).

  int grp_id_;
  // Group ID.

  ACE_Thread_State thr_state_;
  // Current state of the thread.
};

// Forward declaration.
class ACE_Thread_Control;

class ACE_Export ACE_Thread_Manager
  // = TITLE
  //    Manages a pool of threads.
  //
  // = DESCRIPTION
  //    This class allows operations on groups of threads atomically.
{
friend class ACE_Thread_Control;
public:
  enum
  {
    DEFAULT_SIZE = 100
  };

  // = Initialization and termination methods.
  ACE_Thread_Manager (size_t size = ACE_Thread_Manager::DEFAULT_SIZE);
  virtual ~ACE_Thread_Manager (void);

  int open (size_t size = DEFAULT_SIZE); 
  // Initialize the manager with room for SIZE threads.

  int close (void);		
  // Release all resources.

  int spawn (ACE_THR_FUNC func, 
	     void *args = 0,
	     long flags = THR_NEW_LWP, 
	     ACE_thread_t * = 0, 
	     ACE_hthread_t *t_handle = 0,
	     long priority = -1,
	     int grp_id = -1,
	     void *stack = 0, 
	     size_t stack_size = 0);
  // Create a new thread, which executes <func>.  
  // Returns: on success a unique group id that can be used to control
  // other threads added to the same group.  On failure, returns -1.

  int spawn_n (size_t n, 
	       ACE_THR_FUNC func, 
	       void *args = 0,
	       long flags = THR_NEW_LWP,
	       long priority = -1,
	       int grp_id = -1,
	       ACE_Task_Base *task = 0);
  // Create N new threads, all of which execute <func>.  
  // Returns: on success a unique group id that can be used to control
  // all of the threads in the same group.  On failure, returns -1.

  int spawn_n (ACE_thread_t thread_ids[], 
	       size_t n, 
	       ACE_THR_FUNC func, 
	       void *args,
	       long flags, 
	       long priority = -1,
	       int grp_id = -1,
	       void *stack[] = 0,
	       size_t stack_size[] = 0,
	       ACE_hthread_t thread_handles[] = 0);
  // Spawn N new threads, which execute <func> with argument <arg>.
  // The thread_ids of successfully spawned threads will be placed
  // into the <thread_ids> buffer (which must be the same size as
  // <n>).  If <stack> != 0 it is assumed to be an array of <n>
  // pointers to the base of the stacks to use for the threads being
  // spawned.  If <stack_size> != 0 it is assumed to be an array of
  // <n> values indicating how big each of the corresponding <stack>s
  // are.  If <thread_handles> != 0 it is assumed to be an array of
  // <n> thread_handles that will be assigned the values of the thread
  // handles being spawned.  Returns -1 on failure (<errno> will
  // explain...), otherwise returns the group id of the threads.

  void *exit (void *status, int do_thread_exit = 1);
  // Called to clean up when a thread exits.  If <do_thread_exit> is
  // non-0 then <ACE_Thread::exit> is called to exit the thread, in
  // which case <status> is passed as the exit value of the thread.

  int wait (const ACE_Time_Value *timeout = 0);	
  // Block until there are no more threads running in the
  // <Thread_Manager> or <timeout> expires.  Note that <timeout> is
  // treated as "absolute" time.  Returns 0 on success and -1 on
  // failure.

  int wait_grp (int grp_id);
  // Block until there are no more threads running in a group.
  // Returns 0 on success and -1 on failure.

  // = Accessors for ACE_Thread_Descriptors.
  int thread_descriptor (ACE_thread_t, ACE_Thread_Descriptor &);
  // Return the thread descriptor (indexed by ACE_thread_t).  Returns 0 on
  // success and -1 if not found.

  int hthread_descriptor (ACE_hthread_t, ACE_Thread_Descriptor &);
  // Return the thread descriptor (indexed by ACE_hthread_t).  Returns 0
  // on success and -1 if not found.

  int thr_self (ACE_hthread_t &);
  // Passes out the "real" handle to the calling thread, caching it if
  // necessary in TSS to speed up subsequent lookups.  This is
  // necessary since on some platforms (e.g., Win32) we can't get this
  // handle via direct method calls.

  ACE_thread_t thr_self (void);
  // Return the unique ID of the thread.  This is not strictly
  // necessary (because a thread can always just call
  // <ACE_Thread::thr_self>).  However, we put it here to be complete.

  // = Suspend methods, which isn't supported on POSIX pthreads (will not block).
  int suspend_all (void);
  // Suspend all threads 
  int suspend (ACE_thread_t);
  // Suspend a single thread.
  int suspend_grp (int grp_id);
  // Suspend a group of threads.
  int testsuspend (ACE_thread_t t_id);
  // True if <t_id> is inactive (i.e., suspended), else false.

  // = Resume methods, which isn't supported on POSIX pthreads (will not block).
  int resume_all (void);
  // Resume all stopped threads 
  int resume (ACE_thread_t);
  // Resume a single thread.
  int resume_grp (int grp_id);
  // Resume a group of threads.
  int testresume (ACE_thread_t t_id);
  // True if <t_id> is active (i.e., resumed), else false.

  // = Kill methods, send signals -- which isn't supported on Win32 (will not block).
  int kill_all (int signum);
  // Send signum to all stopped threads 
  int kill (ACE_thread_t, int signum);
  // Kill a single thread.
  int kill_grp (int grp_id, int signum);
  // Kill a group of threads.

  // = Cancel methods, which provides a cooperative thread-termination mechanism (will not block).
  int cancel_all (void);
  // Send signum to all stopped threads 
  int cancel (ACE_thread_t);
  // Cancel a single thread.
  int cancel_grp (int grp_id);
  // Cancel a group of threads.
  int testcancel (ACE_thread_t t_id);
  // True if <t_id> is cancelled, else false.

  // = Set/get group ids for a particular thread id.
  int set_grp (ACE_thread_t, int grp_id);
  int get_grp (ACE_thread_t, int &grp_id);

  // = The following methods are new methods which resemble current
  // methods in ACE_Thread Manager. For example, the new apply_task()
  // method resembles the old apply_thr() method, and suspend_task()
  // resembles suspend_thr().
  
  // = Operations on ACE_Tasks.
  int wait_task (ACE_Task_Base *task);
  // Block until there are no more threads running in <task>.  Returns
  // 0 on success and -1 on failure.
  int suspend_task (ACE_Task_Base *task);
  // Suspend all threads in an ACE_Task.
  int resume_task (ACE_Task_Base *task);
  // Resume all threads in an ACE_Task.
  int kill_task (ACE_Task_Base *task, int signum);
  // Kill all threads in an ACE_Task.
  int cancel_task (ACE_Task_Base *task);
  // Cancel all threads in an ACE_Task.

  // = The following method provide new functionality. They do not
  // follow the same design as current methods. They provide new
  // functionality.

  int num_tasks_in_group (int grp_id);
  // Returns the number of <ACE_Task> in a group.

  int num_threads_in_task (ACE_Task_Base *task);
  // Returns the number of threads in an <ACE_Task>.

  int task_list (int grp_id, 
		 ACE_Task_Base *task_list[],
		 size_t n);
  // Returns in <task_list> a list of up to <n> <ACE_Tasks> in a
  // group.  The caller must allocate the memory for <task_list>

  int thread_list (ACE_Task_Base *task, 
		   ACE_thread_t thread_list[],
		   size_t n);
  // Returns in <thread_list> a list of up to <h> thread ids in an
  // <ACE_Task>.  The caller must allocate the memory for
  // <thread_list>.

  int hthread_list (ACE_Task_Base *task, 
		    ACE_hthread_t hthread_list[],
		    size_t n);
  // Returns in <hthread_list> a list of up to <n> thread handles in
  // an <ACE_Task>.  The caller must allocate memory for
  // <hthread_list>.

  // = Set/get group ids for a particular task.
  int set_grp (ACE_Task_Base *task, int grp_id);
  int get_grp (ACE_Task_Base *task, int &grp_id);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  virtual int spawn_i (ACE_THR_FUNC func, 
		       void *args, 
		       long flags, 
		       ACE_thread_t * = 0, 
		       ACE_hthread_t *t_handle = 0,
		       long priority = -1,
		       int grp_id = -1,
		       void *stack = 0, 
		       size_t stack_size = 0,
		       ACE_Task_Base *task = 0);
  // Create a new thread (must be called with locks held).

private:
  int resize (size_t);
  // Resize the pool of Thread_Descriptors.

  int find_thread (ACE_thread_t t_id);
  // Locate the index of the table slot occupied by <t_id>.  Returns
  // -1 if <t_id> is not in the table doesn't contain <t_id>.

  int find_hthread (ACE_hthread_t h_id);
  // Locate the index of the table slot occupied by <h_id>.  Returns
  // -1 if <h_id> is not in the table doesn't contain <h_id>.

  int find_task (ACE_Task_Base *task, int index = -1);
  // Locate the index of the table slot occupied by <task>.  Returns
  // -1 if <task> is not in the table doesn't contain <task>.

  int insert_thr (ACE_thread_t t_id, ACE_hthread_t, int grp_id = -1);
  // Insert a thread in the table (checks for duplicates).

  int append_thr (ACE_thread_t t_id, ACE_hthread_t, 
		  ACE_Thread_State,
		  int grp_id,
		  ACE_Task_Base *task = 0);
  // Append a thread in the table (adds at the end, growing the table
  // if necessary).

  void remove_thr (int i);	
  // Remove thread from the table. 

  // = The following four methods implement a simple scheme for
  // operating on a collection of threads atomically.
  typedef int (ACE_Thread_Manager::*THR_FUNC)(int, int);

  int check_state (ACE_Thread_State state, ACE_thread_t thread);
  // Efficiently check whether <thread> is in a particular <state>.
  // This call updates the TSS cache if possible to speed up
  // subsequent searches.

  int apply_task (ACE_Task_Base *task, THR_FUNC, int = 0);
  // Apply <func> to all members of the table that match the <task>

  int apply_grp (int grp_id, THR_FUNC, int = 0);
  // Apply <func> to all members of the table that match the <grp_id>.

  int apply_all (THR_FUNC, int = 0);
  // Apply <func> to all members of the table.

  int resume_thr (int i);
  // Resume the thread at index <i>.

  int suspend_thr (int i);
  // Suspend the thread at index <i>.

  int kill_thr (int i, int signum);
  // Send signal <signum> to the thread at index <i>.

  int cancel_thr (int i);
  // Set the cancellation flag for the thread at index <i>.

  ACE_Thread_Descriptor *thr_table_;
  // Vector that describes thread state within the Thread_Manager.

  size_t max_table_size_;
  // Maximum number of threads we can manage (should be dynamically
  // allocated).

  size_t current_count_;
  // Current number of threads we are managing.

  int grp_id_;
  // Keeps track of the next group id to assign.

  // = ACE_Thread_Mutex and condition variable for synchronizing termination.
#if defined (ACE_HAS_THREADS)
  ACE_Thread_Mutex lock_;
  ACE_Condition_Thread_Mutex zero_cond_;
#endif /* ACE_HAS_THREADS */
};


class ACE_Export ACE_Thread_Control
  // = TITLE
  //     Used to keep track of a thread's activities within its entry
  //     point function.
{
public:
  ACE_Thread_Control (ACE_Thread_Manager *tm = 0, 
		      int insert = 0);
  // Initialize the thread control object.  If <insert> != 0, then
  // register the thread with the Thread_Manager.

  ~ACE_Thread_Control (void);
  // Remove the thread from its associated <Thread_Manager> and exit
  // the thread if <do_thr_exit> is enabled.

  void *exit (void *status, int do_thr_exit);
  // Remove this thread from its associated <Thread_Manager> and exit
  // the thread if <do_thr_exit> is enabled.

  int insert (ACE_Thread_Manager *tm);
  // Store the <Thread_Manager> and use it to register ourselves for
  // correct shutdown.

  ACE_Thread_Manager *thr_mgr (void);
  // Returns the current <Thread_Manager>.

  ACE_Thread_Manager *thr_mgr (ACE_Thread_Manager *);
  // Atomically set a new <Thread_Manager> and return the old
  // <Thread_Manager>.

  void *status (void *status);
  // Set the exit status (and return existing status).

  void *status (void);
  // Get the current exit status.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Thread_Manager *tm_;
  // Pointer to the thread manager for this block of code.

  void *status_;
  // Keeps track of the exit status for the thread.
};

#if defined (__ACE_INLINE__)
#include "ace/Thread_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_THREAD_MANAGER_H */

