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
#include "ace/Containers.h"

// Forward declarations.
class ACE_Task_Base;
class ACE_Thread_Manager;

class ACE_Export ACE_Thread_Descriptor
  // = Title
  //    Information for controlling threads that run under the control
  //    of the <Thread_Manager>.
{
friend class ACE_Thread_Manager;
friend class ACE_Double_Linked_List<ACE_Thread_Descriptor>;
friend class ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor>;
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

  ACE_Task_Base *task (void);
  // Return the pointer to an <ACE_Task_Base> or NULL if there's no
  // <ACE_Task_Base> associated with this thread.;

  void dump (void) const;
  // Dump the state of an object.

  int at_exit (void *object,
	       ACE_CLEANUP_FUNC cleanup_hook,
	       void *param);
  // Register an object (or array) for cleanup at thread termination.
  // "cleanup_hook" points to a (global, or static member) function
  // that is called for the object or array when it to be destroyed.
  // It may perform any necessary cleanup specific for that object or
  // its class.  "param" is passed as the second parameter to the
  // "cleanup_hook" function; the first parameter is the object (or
  // array) to be destroyed.  Returns 0 on success, non-zero on
  // failure: -1 if virtual memory is exhausted or 1 if the object (or
  // arrayt) had already been registered.

private:
  ACE_thread_t thr_id_;
  // Unique thread ID.
    
  ACE_hthread_t thr_handle_;
  // Unique handle to thread (used by Win32 and AIX).

  int grp_id_;
  // Group ID.

  ACE_Thread_State thr_state_;
  // Current state of the thread.

  ACE_Cleanup_Info cleanup_info_;
  // Stores the cleanup info for a thread.  
  // @@ Note, this should be generalized to be a stack of
  // <ACE_Cleanup_Info>s.

  long flags_;
  // Keeps track of whether this thread was created "detached" or not.
  // If a thread is *not* created detached then if someone calls
  // <ACE_Thread_Manager::wait>, we need to join with that thread (and
  // close down the handle).

  ACE_Task_Base *task_;
  // Pointer to an <ACE_Task_Base> or NULL if there's no <ACE_Task_Base>;

  ACE_Thread_Descriptor *next_;
  ACE_Thread_Descriptor *prev_;
  // We need these pointers to maintain the double-linked list in a
  // thread managers.
};

// Forward declaration.
class ACE_Thread_Control;

// This typedef should be (and used to be) inside the
// ACE_Thread_Manager declaration.  But, it caused compilation
// problems on g++/VxWorks/i960 with -g.  Note that
// ACE_Thread_Manager::THR_FUNC is only used internally in
// ACE_Thread_Manager, so it's not useful for anyone else.
typedef int (ACE_Thread_Manager::*THR_FUNC)(ACE_Thread_Descriptor *, int);

class ACE_Export ACE_Thread_Manager
  // = TITLE
  //    Manages a pool of threads.
  //
  // = DESCRIPTION
  //    This class allows operations on groups of threads atomically.
{
friend class ACE_Thread_Control;
public:
  // = Initialization and termination methods.
  ACE_Thread_Manager (size_t size = 0);
  // <size> is currently unused.
  virtual ~ACE_Thread_Manager (void);

  static ACE_Thread_Manager *instance (void);
  // Get pointer to a process-wide <ACE_Thread_Manager>.

  static ACE_Thread_Manager *instance (ACE_Thread_Manager *);
  // Set pointer to a process-wide <ACE_Thread_Manager> and return
  // existing pointer.

  static void close_singleton (void);
  // Delete the dynamically allocated Singleton

  int open (size_t size = 0);
  // No-op.  Currently unused.

  int close (void);		
  // Release all resources.

  int spawn (ACE_THR_FUNC func, 
	     void *args = 0,
	     long flags = THR_NEW_LWP, 
	     ACE_thread_t * = 0, 
	     ACE_hthread_t *t_handle = 0,
	     long priority = ACE_DEFAULT_THREAD_PRIORITY,
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
	       long priority = ACE_DEFAULT_THREAD_PRIORITY,
	       int grp_id = -1,
	       ACE_Task_Base *task = 0,
	       ACE_hthread_t thread_handles[] = 0);
  // Create N new threads, all of which execute <func>.  
  // Returns: on success a unique group id that can be used to control
  // all of the threads in the same group.  On failure, returns -1.

  int spawn_n (ACE_thread_t thread_ids[], 
	       size_t n, 
	       ACE_THR_FUNC func, 
	       void *args,
	       long flags, 
	       long priority = ACE_DEFAULT_THREAD_PRIORITY,
	       int grp_id = -1,
	       void *stack[] = 0,
	       size_t stack_size[] = 0,
	       ACE_hthread_t thread_handles[] = 0);
  // Spawn N new threads, which execute <func> with argument <arg>.
  // If <thread_ids> != 0 the thread_ids of successfully spawned
  // threads will be placed into the <thread_ids> buffer (which must
  // be the same size as <n>).  If <stack> != 0 it is assumed to be an
  // array of <n> pointers to the base of the stacks to use for the
  // threads being spawned.  If <stack_size> != 0 it is assumed to be
  // an array of <n> values indicating how big each of the
  // corresponding <stack>s are.  If <thread_handles> != 0 it is
  // assumed to be an array of <n> thread_handles that will be
  // assigned the values of the thread handles being spawned.  Returns
  // -1 on failure (<errno> will explain...), otherwise returns the
  // group id of the threads.

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
  ACE_Thread_Descriptor *thread_desc_self (void);
  // Get a pointer to the calling thread's own thread_descriptor.
  // This must be called from a spawn thread.  This function will
  // try to fetch this info from TSS, if it is not cached, then
  // it will look it up and cache it.
  // This function must be called with lock held.

  ACE_Thread_Descriptor *thread_descriptor (ACE_thread_t);
  // Return a pointer to the thread's Thread_Descriptor,
  // 0 if fail.

  ACE_Thread_Descriptor *hthread_descriptor (ACE_hthread_t);
  // Return a pointer to the thread's Thread_Descriptor,
  // 0 if fail.

  int thr_self (ACE_hthread_t &);

  // Return the "real" handle to the calling thread, caching it if
  // necessary in TSS to speed up subsequent lookups. This is
  // necessary since on some platforms (e.g., Win32) we can't get this
  // handle via direct method calls.  Notice that you should *not* close
  // the handle passed back from this method.  It is used internally
  // by Thread Manager.  On the other hand, you *have to* use this internal
  // thread handle when working on Thread_Manager.  Return -1 if fail.

  ACE_thread_t thr_self (void);
  // Return the unique ID of the thread.  This is not strictly
  // necessary (because a thread can always just call
  // <ACE_Thread::thr_self>).  However, we put it here to be complete.

  ACE_Task_Base *task (void);
  // Returns a pointer to the current <ACE_Task_Base> we're executing
  // in if this thread is indeed running in an <ACE_Task_Base>, else
  // return 0.

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
  // Cancel's all the threads.
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
  // methods in <ACE_Thread Manager>. For example, the <apply_task>
  // method resembles the <apply_thr> method, and <suspend_task>
  // resembles <suspend_thr>.
  
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
  // Returns the number of <ACE_Task_Base> in a group.

  int num_threads_in_task (ACE_Task_Base *task);
  // Returns the number of threads in an <ACE_Task_Base>.

  int task_list (int grp_id, 
		 ACE_Task_Base *task_list[],
		 size_t n);
  // Returns in <task_list> a list of up to <n> <ACE_Tasks> in a
  // group.  The caller must allocate the memory for <task_list>

  int thread_list (ACE_Task_Base *task, 
		   ACE_thread_t thread_list[],
		   size_t n);
  // Returns in <thread_list> a list of up to <h> thread ids in an
  // <ACE_Task_Base>.  The caller must allocate the memory for
  // <thread_list>.

  int hthread_list (ACE_Task_Base *task, 
		    ACE_hthread_t hthread_list[],
		    size_t n);
  // Returns in <hthread_list> a list of up to <n> thread handles in
  // an <ACE_Task_Base>.  The caller must allocate memory for
  // <hthread_list>.

  // = Set/get group ids for a particular task.
  int set_grp (ACE_Task_Base *task, int grp_id);
  int get_grp (ACE_Task_Base *task, int &grp_id);

  int at_exit (void *object,
	       ACE_CLEANUP_FUNC cleanup_hook,
	       void *param);
  // Register an object (or array) for cleanup at thread termination.
  // "cleanup_hook" points to a (global, or static member) function
  // that is called for the object or array when it to be destroyed.
  // It may perform any necessary cleanup specific for that object or
  // its class.  "param" is passed as the second parameter to the
  // "cleanup_hook" function; the first parameter is the object (or
  // array) to be destroyed.  "cleanup_hook", for example, may delete
  // the object (or array).

  void dump (void);
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  virtual int spawn_i (ACE_THR_FUNC func, 
		       void *args, 
		       long flags, 
		       ACE_thread_t * = 0, 
		       ACE_hthread_t *t_handle = 0,
		       long priority = ACE_DEFAULT_THREAD_PRIORITY,
		       int grp_id = -1,
		       void *stack = 0, 
		       size_t stack_size = 0,
		       ACE_Task_Base *task = 0);
  // Create a new thread (must be called with locks held).

  void run_thread_exit_hooks (int i);
  // Run the registered hooks when the thread exits.

  ACE_Thread_Descriptor *find_thread (ACE_thread_t t_id);
  // Locate the index of the table slot occupied by <t_id>.  Returns
  // -1 if <t_id> is not in the table doesn't contain <t_id>.

  ACE_Thread_Descriptor *find_hthread (ACE_hthread_t h_id);
  // Locate the index of the table slot occupied by <h_id>.  Returns
  // -1 if <h_id> is not in the table doesn't contain <h_id>.

  ACE_Thread_Descriptor *find_task (ACE_Task_Base *task, int index = -1);
  // Locate the thread descriptor address of the list occupied by
  // <task>.  Returns 0 if <task> is not in the table doesn't contain
  // <task>.

  int insert_thr (ACE_thread_t t_id,
		  ACE_hthread_t,
		  int grp_id = -1,
		  long flags = 0);
  // Insert a thread in the table (checks for duplicates).

  int append_thr (ACE_thread_t t_id, ACE_hthread_t, 
		  ACE_Thread_State,
		  int grp_id,
		  ACE_Task_Base *task = 0,
		  long flags = 0);
  // Append a thread in the table (adds at the end, growing the table
  // if necessary).

  void remove_thr (ACE_Thread_Descriptor *tda);	
  // Remove thread from the table.

  void remove_thr_self(void);
  // Same as above but must be called from the context of the thread
  // that is to be removed.

  // = The following four methods implement a simple scheme for
  // operating on a collection of threads atomically.

  int check_state (ACE_Thread_State state, ACE_thread_t thread);
  // Efficiently check whether <thread> is in a particular <state>.
  // This call updates the TSS cache if possible to speed up
  // subsequent searches.

  int apply_task (ACE_Task_Base *task, THR_FUNC, int  = 0);
  // Apply <func> to all members of the table that match the <task>

  int apply_grp (int grp_id, THR_FUNC, int  = 0);
  // Apply <func> to all members of the table that match the <grp_id>.

  int apply_all (THR_FUNC, int  = 0);
  // Apply <func> to all members of the table.

  int join_thr (ACE_Thread_Descriptor *tda);
  // Join the thread described in <tda>.

  int resume_thr (ACE_Thread_Descriptor *tda);
  // Resume the thread described in <tda>.

  int suspend_thr (ACE_Thread_Descriptor *tda);
  // Suspend the thread described in <tda>.

  int kill_thr (ACE_Thread_Descriptor *tda, int signum);
  // Send signal <signum> to the thread described in <tda>.

  int cancel_thr (ACE_Thread_Descriptor *tda);
  // Set the cancellation flag for the thread described in <tda>.

  ACE_Double_Linked_List<ACE_Thread_Descriptor> thr_list_;
  // Keeping a list of thread descriptors within the thread manager.
  // Double-linked list enables us to cache the entries in TSS
  // and adding/removing thread descriptor entries without
  // affecting other thread's descriptor entries.

#if !defined (VXWORKS)
  ACE_Unbounded_Queue<ACE_Thread_Descriptor> terminated_thr_queue_;
  // Collect terminated but not yet joined thread entries.
#endif /* VXWORKS */

  int grp_id_;
  // Keeps track of the next group id to assign.

  // = ACE_Thread_Mutex and condition variable for synchronizing termination.
#if defined (ACE_HAS_THREADS)
  ACE_Thread_Mutex lock_;
  // Serialize access to the <zero_cond_>.

  ACE_Condition_Thread_Mutex zero_cond_;
  // Keep track of when there are no more threads.
#endif /* ACE_HAS_THREADS */

private:
  static ACE_Thread_Manager *thr_mgr_;
  // Pointer to a process-wide <ACE_Thread_Manager>.

  static int delete_thr_mgr_;
  // Must delete the <thr_mgr_> if non-0.
};

class ACE_Export ACE_Thread_Control
  // = TITLE
  //     Used to keep track of a thread's activities within its entry
  //     point function.
  //
  // = DESCRIPTION
  //     A <ACE_Thread_Manager> uses this class to ensure that threads
  //     it spawns automatically register and unregister themselves
  //     with it.
  //
  //     This class can be stored in thread-specific storage using the
  //     <ACE_TSS> wrapper.  When a thread exits the
  //     <ACE_TSS::cleanup> function deletes this object, thereby
  //     ensuring that it gets removed from its associated
  //     <ACE_Thread_Manager>.
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

class ACE_Export ACE_Thread_Exit
  // = TITLE
  //    Keep exit information for a Thread in thread specific storage.
  //    so that the thread-specific exit hooks will get called no
  //    matter how the thread exits (e.g., via <ACE_Thread::exit>, C++
  //    or Win32 exception, "falling off the end" of the thread entry
  //    point function, etc.).
  //
  // = DESCRIPTION
  //    This clever little helper class is stored in thread-specific
  //    storage using the <ACE_TSS> wrapper.  When a thread exits the
  //    <ACE_TSS::cleanup> function deletes this object, thereby
  //    closing it down gracefully.
{
public:
  ACE_Thread_Exit (void);
  // Capture the Thread that will be cleaned up automatically.

  void thr_mgr (ACE_Thread_Manager *tm);
  // Set the <ACE_Thread_Manager>.

  void *status (void *s);
  // Set the exit status.

  void *status (void);
  // Get the exit status.

  ~ACE_Thread_Exit (void);
  // Destructor calls the thread-specific exit hooks when a thread
  // exits.

  static ACE_Thread_Exit *instance (void);
  // Singleton access point.

  static void cleanup (void *instance, void *);
  // Cleanup method, used by the <ACE_Object_Manager> to destroy the
  // singleton.

private:
  void *status_;
  // Exit status...

  ACE_Thread_Control thread_control_;
  // Automatically add/remove the thread from the
  // <ACE_Thread_Manager>.
};

#if defined (__ACE_INLINE__)
#include "ace/Thread_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_THREAD_MANAGER_H */

