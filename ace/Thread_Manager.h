/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Thread_Manager.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_THREAD_MANAGER_H
#define ACE_THREAD_MANAGER_H
#include "ace/pre.h"

#include "ace/Thread.h"
#include "ace/Thread_Adapter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Containers.h"
#include "ace/Free_List.h"
#include "ace/Singleton.h"
#include "ace/Log_Msg.h"

// The following macros control how a Thread Manager manages a pool of
// Thread_Descriptor.  Currently, the default behavior is not to
// preallocate any thread descriptor and never (well, almost never)
// free up any thread descriptor until the Thread Manager gets
// destructed.  Which means, once your system is stable, you rarely
// need to pay the price of memory allocation.  On a deterministic
// system, which means, the number of threads spawned can be
// determined before hand, you can either redefine the memory pool
// size macros to suit your need or constructed the Thread_Manager
// accordingly.  That way, you don't pay the price of memory
// allocation when the system is really doing its job.  OTOH, on
// system with resources constraint, you may want to lower the size of
// ACE_DEFAULT_THREAD_MANAGER_HWM to avoid unused memory hanging
// around.

#if !defined (ACE_DEFAULT_THREAD_MANAGER_PREALLOC)
# define ACE_DEFAULT_THREAD_MANAGER_PREALLOC 0
#endif /* ACE_DEFAULT_THREAD_MANAGER_PREALLOC */

#if !defined (ACE_DEFAULT_THREAD_MANAGER_LWM)
# define ACE_DEFAULT_THREAD_MANAGER_LWM 1
#endif /* ACE_DEFAULT_THREAD_MANAGER_LWM */

#if !defined (ACE_DEFAULT_THREAD_MANAGER_INC)
# define ACE_DEFAULT_THREAD_MANAGER_INC 1
#endif /* ACE_DEFAULT_THREAD_MANAGER_INC */

#if !defined (ACE_DEFAULT_THREAD_MANAGER_HWM)
# define ACE_DEFAULT_THREAD_MANAGER_HWM ACE_DEFAULT_FREE_LIST_HWM
// this is a big number
#endif /* ACE_DEFAULT_THREAD_MANAGER_HWM */

// This is the synchronization mechanism used to prevent a thread
// descriptor gets removed from the Thread_Manager before it gets
// stash into it.  If you want to disable this feature (and risk of
// corrupting the freelist,) you define the lock as ACE_Null_Mutex.
// Usually, if you can be sure that your threads will run for an
// extended period of time, you can safely disable the lock.

#if !defined (ACE_DEFAULT_THREAD_MANAGER_LOCK)
# define ACE_DEFAULT_THREAD_MANAGER_LOCK ACE_SYNCH_MUTEX
#endif /* ACE_DEFAULT_THREAD_MANAGER_LOCK */

// Forward declarations.
class ACE_Task_Base;
class ACE_Thread_Manager;
class ACE_Thread_Descriptor;

#if !defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
  /**
   * @class ACE_At_Thread_Exit
   *
   * @brief Contains a method to be applied when a thread is terminated.
   */
class ACE_Export ACE_At_Thread_Exit
{
  friend class ACE_Thread_Descriptor;
  friend class ACE_Thread_Manager;
public:
  // Default constructor
  ACE_At_Thread_Exit (void);

  // The destructor
  virtual ~ACE_At_Thread_Exit (void);

  // <At_Thread_Exit> has the ownership?
  int is_owner (void) const;

  // Set the ownership of the <At_Thread_Exit>.
  int is_owner (int owner);

  // This <At_Thread_Exit> was applied?
  int was_applied (void) const;

  // Set applied state of <At_Thread_Exit>.
  int was_applied (int applied);

protected:
  /// The next <At_Thread_Exit> hook in the list.
  ACE_At_Thread_Exit *next_;

  // Do the apply if necessary
  void do_apply (void);

  /// The apply method.
  virtual void apply (void) = 0;

  /// The Thread_Descriptor where this at is registered.
  ACE_Thread_Descriptor* td_;

  /// The at was applied?
  int was_applied_;

  /// The at has the ownership of this?
  int is_owner_;
};

class ACE_Export ACE_At_Thread_Exit_Func : public ACE_At_Thread_Exit
{
public:
   // Constructor
   ACE_At_Thread_Exit_Func (void *object,
                            ACE_CLEANUP_FUNC func,
                            void *param = 0);

  virtual ~ACE_At_Thread_Exit_Func (void);

protected:
   /// The object to be cleanup
   void *object_;

   /// The cleanup func
   ACE_CLEANUP_FUNC func_;

   /// A param if required
   void *param_;

   // The apply method
   void apply (void);
};

#endif /* !ACE_USE_ONE_SHOT_AT_THREAD_EXIT */

/**
 * @class ACE_Thread_Descriptor_Base
 *
 * @brief Basic information for thread descriptors.  These information
 * gets extracted out because we need it after a thread is
 * terminated.
 */
class ACE_Export ACE_Thread_Descriptor_Base : public ACE_OS_Thread_Descriptor
{

  friend class ACE_Thread_Manager;
  friend class ACE_Double_Linked_List<ACE_Thread_Descriptor_Base>;
  friend class ACE_Double_Linked_List_Iterator_Base<ACE_Thread_Descriptor_Base>;
  friend class ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor_Base>;
  friend class ACE_Double_Linked_List<ACE_Thread_Descriptor>;
  friend class ACE_Double_Linked_List_Iterator_Base<ACE_Thread_Descriptor>;
  friend class ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor>;
public:
  ACE_Thread_Descriptor_Base (void);
  ~ACE_Thread_Descriptor_Base (void);

  // = We need the following operators to make Borland happy.

  /// Equality operator.
  int operator== (const ACE_Thread_Descriptor_Base &rhs) const;

  /// Inequality operator.
  int operator!= (const ACE_Thread_Descriptor_Base &rhs) const;

  /// Group ID.
  int grp_id (void) const;

  /// Current state of the thread.
  ACE_UINT32 state (void) const;

  /// Return the pointer to an <ACE_Task_Base> or NULL if there's no
  /// <ACE_Task_Base> associated with this thread.;
  ACE_Task_Base *task (void) const;

protected:
  /// Reset this base thread descriptor.
  void reset (void);

  /// Unique thread ID.
  ACE_thread_t thr_id_;

  /// Unique handle to thread (used by Win32 and AIX).
  ACE_hthread_t thr_handle_;

  /// Group ID.
  int grp_id_;

  /// Current state of the thread.
  ACE_UINT32 thr_state_;

  /// Pointer to an <ACE_Task_Base> or NULL if there's no
  /// <ACE_Task_Base>.
  ACE_Task_Base *task_;

  /// We need these pointers to maintain the double-linked list in a
  /// thread managers.
  ACE_Thread_Descriptor_Base *next_;
  ACE_Thread_Descriptor_Base *prev_;
};

/**
 * @class ACE_Thread_Descriptor
 *
 * @brief Information for controlling threads that run under the control
 * of the <Thread_Manager>.
 */
class ACE_Export ACE_Thread_Descriptor : public ACE_Thread_Descriptor_Base
{
#if !defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
  friend class ACE_At_Thread_Exit;
#endif /* !ACE_USE_ONE_SHOT_AT_THREAD_EXIT */
  friend class ACE_Thread_Manager;
  friend class ACE_Double_Linked_List<ACE_Thread_Descriptor>;
  friend class ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor>;
public:
  // = Initialization method.
  ACE_Thread_Descriptor (void);

  // = Accessor methods.
  /// Unique thread id.
  ACE_thread_t self (void) const;

  /// Unique handle to thread (used by Win32 and AIX).
  void self (ACE_hthread_t &);

  /// Dump the state of an object.
  void dump (void) const;

#if !defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
  /**
   * This cleanup function must be called only for ACE_TSS_cleanup.
   * The ACE_TSS_cleanup delegate Log_Msg instance destruction when
   * Log_Msg cleanup is called before terminate.
   */
  void log_msg_cleanup(ACE_Log_Msg* log_msg);

  /**
   * Register an At_Thread_Exit hook and the ownership is acquire by
   * Thread_Descriptor, this is the usual case when the AT is dynamically
   * allocated.
   */
  int at_exit (ACE_At_Thread_Exit* cleanup);

  /// Register an At_Thread_Exit hook and the ownership is retained for the
  /// caller. Normally used when the at_exit hook is created in stack.
  int at_exit (ACE_At_Thread_Exit& cleanup);
#endif /* !ACE_USE_ONE_SHOT_AT_THREAD_EXIT */

  /**
   * Register an object (or array) for cleanup at thread termination.
   * "cleanup_hook" points to a (global, or static member) function
   * that is called for the object or array when it to be destroyed.
   * It may perform any necessary cleanup specific for that object or
   * its class.  "param" is passed as the second parameter to the
   * "cleanup_hook" function; the first parameter is the object (or
   * array) to be destroyed.  Returns 0 on success, non-zero on
   * failure: -1 if virtual memory is exhausted or 1 if the object (or
   * arrayt) had already been registered.
   */
  int at_exit (void *object,
               ACE_CLEANUP_FUNC cleanup_hook,
               void *param);

  /// Do nothing destructor to keep some compilers happy
  ~ACE_Thread_Descriptor (void);

  /**
   * Do nothing but to acquire the thread descriptor's lock and
   * release.  This will first check if the thread is registered or
   * not.  If it is already registered, there's no need to reacquire
   * the lock again.  This is used mainly to get newly spawned thread
   * in synch with thread manager and prevent it from accessing its
   * thread descriptor before it gets fully built.  This function is
   * only called from ACE_Log_Msg::thr_desc.
   */
  void acquire_release (void);

  /**
   * Set/get the <next_> pointer.  These are required by the
   * ACE_Free_List.  ACE_INLINE is specified here because one version
   * of g++ couldn't grok this code without it.
   */
  ACE_INLINE_FOR_GNUC void set_next (ACE_Thread_Descriptor *td);
  ACE_INLINE_FOR_GNUC ACE_Thread_Descriptor *get_next (void) const;

private:
  /// Reset this thread descriptor.
  void reset (ACE_Thread_Manager *tm);

#if !defined (ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
  /// Pop an At_Thread_Exit from at thread termination list, apply the at
  /// if apply is true.
  void at_pop (int apply = 1);

  /// Push an At_Thread_Exit to at thread termination list and set the
  /// ownership of at.
  void at_push (ACE_At_Thread_Exit* cleanup,
                int is_owner = 0);

  /// Run the AT_Thread_Exit hooks.
  void do_at_exit (void);

  /// terminate realize the cleanup process to thread termination
  void terminate (void);

  /// Thread_Descriptor is the ownership of ACE_Log_Msg if log_msg_!=0
  /// This can occur because ACE_TSS_cleanup was executed before terminate.
  ACE_Log_Msg *log_msg_;

  /// The AT_Thread_Exit list
  ACE_At_Thread_Exit *at_exit_list_;
#endif  /* !ACE_USE_ONE_SHOT_AT_THREAD_EXIT */

  /**
   * Stores the cleanup info for a thread.
   * @@ Note, this should be generalized to be a stack of
   * <ACE_Cleanup_Info>s.
   */
  ACE_Cleanup_Info cleanup_info_;

#if !defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
  /// Pointer to an <ACE_Thread_Manager> or NULL if there's no
  /// <ACE_Thread_Manager>.
  ACE_Thread_Manager* tm_;
#endif /* !ACE_USE_ONE_SHOT_AT_THREAD_EXIT */

  /// Registration lock to prevent premature removal of thread descriptor.
  ACE_DEFAULT_THREAD_MANAGER_LOCK *sync_;

#if !defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
  /// Keep track of termination status.
  int terminated_;
#endif  /* !ACE_USE_ONE_SHOT_AT_THREAD_EXIT */
};

// Forward declaration.
class ACE_Thread_Control;

// This typedef should be (and used to be) inside the
// ACE_Thread_Manager declaration.  But, it caused compilation
// problems on g++/VxWorks/i960 with -g.  Note that
// ACE_Thread_Manager::THR_FUNC is only used internally in
// ACE_Thread_Manager, so it's not useful for anyone else.
// It also caused problems on IRIX5 with g++.
#if defined (__GNUG__)
typedef int (ACE_Thread_Manager::*ACE_THR_MEMBER_FUNC)(ACE_Thread_Descriptor *, int);
#endif /* __GNUG__ */

/**
 * @class ACE_Thread_Manager
 *
 * @brief Manages a pool of threads.
 *
 * This class allows operations on groups of threads atomically.
 * The default behavior of thread manager is to wait on
 * all threads under it's management when it gets destructed.
 * Therefore, remember to remove a thread from thread manager if
 * you don't want it to wait for the thread. There are also
 * function to disable this default wait-on-exit behavior.
 * However, if your program depends on turning this off to run
 * correctly, you are probably doing something wrong.  Rule of
 * thumb, use ACE_Thread to manage your daemon threads.
 * Notice that if there're threads live beyond the scope of
 * <main>, you are sure to have resource leaks in your program.
 * Remember to wait on threads before exiting <main> if that
 * could happen in your programs.
 */
class ACE_Export ACE_Thread_Manager
{
public:
  friend class ACE_Thread_Control;
#if !defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
  friend class ACE_Thread_Descriptor;
#endif /* !ACE_USE_ONE_SHOT_AT_THREAD_EXIT */

#if !defined (__GNUG__)
  typedef int (ACE_Thread_Manager::*ACE_THR_MEMBER_FUNC)(ACE_Thread_Descriptor *, int);
#endif /* !__GNUG__ */

  // These are the various states a thread managed by the
  // <Thread_Manager> can be in.
  enum
  {
    /// Uninitialized.
    ACE_THR_IDLE = 0x00000000,

    /// Created but not yet running.
    ACE_THR_SPAWNED = 0x00000001,

    /// Thread is active (naturally, we don't know if it's actually
    /// *running* because we aren't the scheduler...).
    ACE_THR_RUNNING = 0x00000002,

    /// Thread is suspended.
    ACE_THR_SUSPENDED = 0x00000004,

    /// Thread has been cancelled (which is an indiction that it needs to
    /// terminate...).
    ACE_THR_CANCELLED = 0x00000008,

    /// Thread has shutdown, but the slot in the thread manager hasn't
    /// been reclaimed yet.
    ACE_THR_TERMINATED = 0x00000010,

    /// Join operation has been invoked on the thread by thread manager.
    ACE_THR_JOINING = 0x10000000
  };

  // = Initialization and termination methods.
  /**
   * @breif Initialization and termination methods.
   *
   * Internally, ACE_Thread_Manager keeps a freelist for caching
   * resources it uses to keep track of managed threads (not the
   * threads themselves.)  @a prealloc, @a lwm, @a inc, @hwm
   * determine the initial size, the low water mark, increment step,
   * and high water mark of the freelist.
   *
   * @sa ACE_Free_List
   */
  ACE_Thread_Manager (size_t preaolloc = ACE_DEFAULT_THREAD_MANAGER_PREALLOC,
                      size_t lwm = ACE_DEFAULT_THREAD_MANAGER_LWM,
                      size_t inc = ACE_DEFAULT_THREAD_MANAGER_INC,
                      size_t hwm = ACE_DEFAULT_THREAD_MANAGER_HWM);
  virtual ~ACE_Thread_Manager (void);

#if ! defined (ACE_THREAD_MANAGER_LACKS_STATICS)
  /// Get pointer to a process-wide <ACE_Thread_Manager>.
  static ACE_Thread_Manager *instance (void);

  /// Set pointer to a process-wide <ACE_Thread_Manager> and return
  /// existing pointer.
  static ACE_Thread_Manager *instance (ACE_Thread_Manager *);

  /// Delete the dynamically allocated Singleton
  static void close_singleton (void);
#endif /* ! defined (ACE_THREAD_MANAGER_LACKS_STATICS) */

  /// No-op.  Currently unused.
  int open (size_t size = 0);

  /**
   * Release all resources.
   * By default, this method will wait till all threads
   * exit.  However, when called from <close_singleton>, most global resources
   * are destroyed and thus, we don't try to wait but just clean up the thread
   * descriptor list.
   */
  int close (void);

  // The <ACE_thread_t> * argument to each of the <spawn> family member
  // functions is interpreted and used as shown in the following
  // table.  NOTE:  the final option, to provide task names, is _only_
  // supported on VxWorks!
  //
  // Value of ACE_thread_t * argument  Use                         Platforms
  // ================================  ==========================  =========
  // 0                                 Not used.                   All
  // non-0 (and points to 0 char *     The task name is passed     All
  //   on VxWorks)                       back in the char *.
  // non-0, points to non-0 char *     The char * is used as       VxWorks only
  //                                     the task name.  The
  //                                     argument is not modified.

  /**
   * Create a new thread, which executes <func>.
   * Returns: on success a unique group id that can be used to control
   * other threads added to the same group.  On failure, returns -1.
   */
  int spawn (ACE_THR_FUNC func,
             void *args = 0,
             long flags = THR_NEW_LWP | THR_JOINABLE,
             ACE_thread_t * = 0,
             ACE_hthread_t *t_handle = 0,
             long priority = ACE_DEFAULT_THREAD_PRIORITY,
             int grp_id = -1,
             void *stack = 0,
             size_t stack_size = 0);

  /**
   * Create N new threads, all of which execute <func>.
   * Returns: on success a unique group id that can be used to control
   * all of the threads in the same group.  On failure, returns -1.
   */
  int spawn_n (size_t n,
               ACE_THR_FUNC func,
               void *args = 0,
               long flags = THR_NEW_LWP | THR_JOINABLE,
               long priority = ACE_DEFAULT_THREAD_PRIORITY,
               int grp_id = -1,
               ACE_Task_Base *task = 0,
               ACE_hthread_t thread_handles[] = 0,
               void *stack[] = 0,
               size_t stack_size[] = 0);

  /**
   * Spawn N new threads, which execute <func> with argument <arg>.
   * If <thread_ids> != 0 the thread_ids of successfully spawned
   * threads will be placed into the <thread_ids> buffer (which must
   * be the same size as <n>).  If <stack> != 0 it is assumed to be an
   * array of <n> pointers to the base of the stacks to use for the
   * threads being spawned.  If <stack_size> != 0 it is assumed to be
   * an array of <n> values indicating how big each of the
   * corresponding <stack>s are.  If <thread_handles> != 0 it is
   * assumed to be an array of <n> thread_handles that will be
   * assigned the values of the thread handles being spawned.
   *
   * Threads in Thread_Manager can be maniputated in groups based on
   * <grp_id> or <task> using functions such as kill_grp() or
   * cancel_task().
   *
   * If <grp_id> is assigned, the newly spawned threads are added into
   * the group.  Otherwise, the Thread_Manager assigns these <n>
   * threads with a grp_id.  You should choose either assigning
   * <grp_id> everytime, or let the Thread_Manager handles it for
   * you consistently.
   *
   * The argument <task> is usually assigned by
   * ACE_Task_Base::activate.  It associate the newly spawned threads
   * with an ACE_Task instance.
   *
   * @retval -1 on failure (<errno> will explain...), otherwise returns the
   * group id of the threads.  */
  int spawn_n (ACE_thread_t thread_ids[],
               size_t n,
               ACE_THR_FUNC func,
               void *args,
               long flags,
               long priority = ACE_DEFAULT_THREAD_PRIORITY,
               int grp_id = -1,
               void *stack[] = 0,
               size_t stack_size[] = 0,
               ACE_hthread_t thread_handles[] = 0,
               ACE_Task_Base *task = 0);

  /**
   * Called to clean up when a thread exits.  If <do_thread_exit> is
   * non-0 then <ACE_Thread::exit> is called to exit the thread, in
   * which case <status> is passed as the exit value of the thread.
   * Should _not_ be called by main thread.
   */
  void *exit (void *status = 0,
              int do_thread_exit = 1);

  /**
   * Block until there are no more threads running in the
   * <Thread_Manager> or <timeout> expires.  Note that <timeout> is
   * treated as "absolute" time.  Returns 0 on success and -1 on
   * failure.  If <abandon_detached_threads> is set, wait will first
   * check thru its thread list for threads with THR_DETACHED or
   * THR_DAEMON flags set and remove these threads.  Notice that
   * unlike other wait_* function, by default, <wait> does wait on
   * all thread spawned by this thread_manager no matter the detached
   * flags are set or not unless it is called with
   * <abandon_detached_threads> flag set.
   * NOTE that if this function is called while the ACE_Object_Manager
   * is shutting down (as a result of program rundown via ACE::fini),
   * it will not wait for any threads to complete. If you must wait for
   * threads spawned by this thread manager to complete and you are in a
   * ACE rundown situation (such as your object is being destroyed by the
   * ACE_Object_Manager) you can use wait_grp instead.
   */
  int wait (const ACE_Time_Value *timeout = 0,
            int abandon_detached_threads = 0);

  /// Join a thread specified by <tid>.  Do not wait on a detached thread.
  int join (ACE_thread_t tid, void **status = 0);

  /**
   * Block until there are no more threads running in a group.
   * Returns 0 on success and -1 on failure.  Notice that wait_grp
   * will not wait on detached threads.
   */
  int wait_grp (int grp_id);

  // = Accessors for ACE_Thread_Descriptors.
  /**
   * Get a pointer to the calling thread's own thread_descriptor.
   * This must be called from a spawn thread.  This function will
   * fetch the info from TSS.
   */
  ACE_Thread_Descriptor *thread_desc_self (void);

  /// Return a pointer to the thread's Thread_Descriptor,
  /// 0 if fail.
  ACE_Thread_Descriptor *thread_descriptor (ACE_thread_t);

  /// Return a pointer to the thread's Thread_Descriptor,
  /// 0 if fail.
  ACE_Thread_Descriptor *hthread_descriptor (ACE_hthread_t);

  /**
   * Return the "real" handle to the calling thread, caching it if
   * necessary in TSS to speed up subsequent lookups. This is
   * necessary since on some platforms (e.g., Win32) we can't get this
   * handle via direct method calls.  Notice that you should *not*
   * close the handle passed back from this method.  It is used
   * internally by Thread Manager.  On the other hand, you *have to*
   * use this internal thread handle when working on Thread_Manager.
   * Return -1 if fail.
   */
  int thr_self (ACE_hthread_t &);

  /**
   * Return the unique ID of the thread.  This is not strictly
   * necessary (because a thread can always just call
   * <ACE_Thread::self>).  However, we put it here to be complete.
   */
  ACE_thread_t thr_self (void);

  /**
   * Returns a pointer to the current <ACE_Task_Base> we're executing
   * in if this thread is indeed running in an <ACE_Task_Base>, else
   * return 0.
   */
  ACE_Task_Base *task (void);

  // = Suspend methods, which isn't supported on POSIX pthreads (will not block).
  /**
   * Suspend all threads
   * Suspend a single thread.
   * Suspend a group of threads.
   * True if <t_id> is inactive (i.e., suspended), else false.
   */
  int suspend_all (void);
  int suspend (ACE_thread_t);
  int suspend_grp (int grp_id);
  int testsuspend (ACE_thread_t t_id);

  // = Resume methods, which isn't supported on POSIX pthreads (will not block).
  /**
   * Resume all stopped threads
   * Resume a single thread.
   * Resume a group of threads.
   * True if <t_id> is active (i.e., resumed), else false.
   */
  int resume_all (void);
  int resume (ACE_thread_t);
  int resume_grp (int grp_id);
  int testresume (ACE_thread_t t_id);

  // = Send signals to one or more threads without blocking.
  /**
   * Send <signum> to all stopped threads.  Not supported on platforms
   * that do not have advanced signal support, such as Win32.
   * Send the <signum> to a single thread.  Not supported on platforms
   * that do not have advanced signal support, such as Win32.
   * Send <signum> to a group of threads, not supported on platforms
   * that do not have advanced signal support, such as Win32.
   */
  int kill_all (int signum);
  int kill (ACE_thread_t,
            int signum);
  int kill_grp (int grp_id,
                int signum);

  // = Cancel methods, which provides a cooperative thread-termination mechanism (will not block).
  /**
   * Cancel's all the threads.
   * Cancel a single thread.
   * Cancel a group of threads.
   * True if <t_id> is cancelled, else false.
   */
  int cancel_all (int async_cancel = 0);
  int cancel (ACE_thread_t, int async_cancel = 0);
  int cancel_grp (int grp_id, int async_cancel = 0);
  int testcancel (ACE_thread_t t_id);

  // = Set/get group ids for a particular thread id.
  int set_grp (ACE_thread_t,
               int grp_id);
  int get_grp (ACE_thread_t,
               int &grp_id);

  // = The following methods are new methods which resemble current
  // methods in <ACE_Thread Manager>. For example, the <apply_task>
  // method resembles the <apply_thr> method, and <suspend_task>
  // resembles <suspend_thr>.

  // = Operations on ACE_Tasks.

  /**
   * Block until there are no more threads running in <task>.  Returns
   * 0 on success and -1 on failure.  Note that <wait_task> will not
   * wait on detached threads.
   * Suspend all threads in an ACE_Task.
   * Resume all threads in an ACE_Task.
   * Send a signal <signum> to all threads in an <ACE_Task>.
   */
  int wait_task (ACE_Task_Base *task);
  int suspend_task (ACE_Task_Base *task);
  int resume_task (ACE_Task_Base *task);
  int kill_task (ACE_Task_Base *task,
                 int signum);

  /**
   * Cancel all threads in an <ACE_Task>.  If <async_cancel> is non-0,
   * then asynchronously cancel these threads if the OS platform
   * supports cancellation.  Otherwise, perform a "cooperative"
   * cancellation.
   */
  int cancel_task (ACE_Task_Base *task, int async_cancel = 0);

  // = Collect thread handles in the thread manager.  Notice that
  //   the collected information is just a snapshot.
  /// Check if the thread is managed by the thread manager.  Return true if
  /// the thread is found, false otherwise.
  int hthread_within (ACE_hthread_t handle);
  int thread_within (ACE_thread_t tid);

  /// Returns the number of <ACE_Task_Base> in a group.
  int num_tasks_in_group (int grp_id);

  /// Returns the number of threads in an <ACE_Task_Base>.
  int num_threads_in_task (ACE_Task_Base *task);

  /**
   * Returns in <task_list> a list of up to <n> <ACE_Tasks> in a
   * group.  The caller must allocate the memory for <task_list>.  In
   * case of an error, -1 is returned. If no requested values are
   * found, 0 is returned, otherwise correct number of retrieved
   * values are returned.
   */
  int task_list (int grp_id,
                 ACE_Task_Base *task_list[],
                 size_t n);

  /**
   * Returns in <thread_list> a list of up to <n> thread ids in an
   * <ACE_Task_Base>.  The caller must allocate the memory for
   * <thread_list>.  In case of an error, -1 is returned. If no
   * requested values are found, 0 is returned, otherwise correct
   * number of retrieved values are returned.
   */
  int thread_list (ACE_Task_Base *task,
                   ACE_thread_t thread_list[],
                   size_t n);

  /**
   * Returns in <hthread_list> a list of up to <n> thread handles in
   * an <ACE_Task_Base>.  The caller must allocate memory for
   * <hthread_list>.  In case of an error, -1 is returned. If no
   * requested values are found, 0 is returned, otherwise correct
   * number of retrieved values are returned.
   */
  int hthread_list (ACE_Task_Base *task,
                    ACE_hthread_t hthread_list[],
                    size_t n);

  /**
   * Returns in <thread_list> a list of up to <n> thread ids in a
   * group <grp_id>.  The caller must allocate the memory for
   * <thread_list>.  In case of an error, -1 is returned. If no
   * requested values are found, 0 is returned, otherwise correct
   * number of retrieved values are returned.
   */
  int thread_grp_list (int grp_id,
                       ACE_thread_t thread_list[],
                       size_t n);

  /**
   * Returns in <hthread_list> a list of up to <n> thread handles in
   * a group <grp_id>.  The caller must allocate memory for
   * <hthread_list>.
   */
  int hthread_grp_list (int grp_id,
                        ACE_hthread_t hthread_list[],
                        size_t n);

  /**
   * Returns in <task_list> a list of up to <n> <ACE_Tasks>.  The
   * caller must allocate the memory for <task_list>.  In case of an
   * error, -1 is returned. If no requested values are found, 0 is
   * returned, otherwise correct number of retrieved values are
   * returned.
   */
  int task_all_list (ACE_Task_Base *task_list[],
                     size_t n);

  /**
   * Returns in <thread_list> a list of up to <n> thread ids.  The
   * caller must allocate the memory for <thread_list>.  In case of an
   * error, -1 is returned. If no requested values are found, 0 is
   * returned, otherwise correct number of retrieved values are
   * returned.
   */
  int thread_all_list (ACE_thread_t thread_list[],
                       size_t n);

  // = Set/get group ids for a particular task.
  int set_grp (ACE_Task_Base *task, int grp_id);
  int get_grp (ACE_Task_Base *task, int &grp_id);

  /// Return a count of the current number of threads active in the
  /// <Thread_Manager>.
  int count_threads (void) const;

#if !defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
  /**
   * Register an At_Thread_Exit hook and the ownership is acquire by
   * Thread_Descriptor, this is the usual case when the AT is dynamically
   * allocated.
   */
  int at_exit (ACE_At_Thread_Exit* cleanup);

  /// Register an At_Thread_Exit hook and the ownership is retained for the
  /// caller. Normally used when the at_exit hook is created in stack.
  int at_exit (ACE_At_Thread_Exit& cleanup);
#endif /* !ACE_USE_ONE_SHOT_AT_THREAD_EXIT */

  /**
   *
   *****
   * @deprecated This function is deprecated.  Please use the previous two
   *    at_exit method.  Notice that you should avoid mixing this method
   *    with the previous two at_exit methods.
   *****
   *
   * Register an object (or array) for cleanup at
   * thread termination.  "cleanup_hook" points to a (global, or
   * static member) function that is called for the object or array
   * when it to be destroyed.  It may perform any necessary cleanup
   * specific for that object or its class.  "param" is passed as the
   * second parameter to the "cleanup_hook" function; the first
   * parameter is the object (or array) to be destroyed.
   * "cleanup_hook", for example, may delete the object (or array).
   * If <cleanup_hook> == 0, the <object> will _NOT_ get cleanup at
   * thread exit.  You can use this to cancel the previously added
   * at_exit.
   */
  int at_exit (void *object,
               ACE_CLEANUP_FUNC cleanup_hook,
               void *param);

  /// Access function to determine whether the Thread_Manager will
  /// wait for its thread to exit or not when being closing down.
  void wait_on_exit (int dowait);
  int wait_on_exit (void);

  /// Dump the state of an object.
  void dump (void);

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Create a new thread (must be called with locks held).
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

  /// Run the registered hooks when the thread exits.
  void run_thread_exit_hooks (int i);

  /// Locate the index of the table slot occupied by <t_id>.  Returns
  /// -1 if <t_id> is not in the table doesn't contain <t_id>.
  ACE_Thread_Descriptor *find_thread (ACE_thread_t t_id);

  /// Locate the index of the table slot occupied by <h_id>.  Returns
  /// -1 if <h_id> is not in the table doesn't contain <h_id>.
  ACE_Thread_Descriptor *find_hthread (ACE_hthread_t h_id);

  /**
   * Locate the thread descriptor address of the list occupied by
   * <task>.  Returns 0 if <task> is not in the table doesn't contain
   * <task>.
   */
  ACE_Thread_Descriptor *find_task (ACE_Task_Base *task,
                                    int slot = -1);

  /// Insert a thread in the table (checks for duplicates).
  int insert_thr (ACE_thread_t t_id,
                  ACE_hthread_t,
                  int grp_id = -1,
                  long flags = 0);

  /// Append a thread in the table (adds at the end, growing the table
  /// if necessary).
  int append_thr (ACE_thread_t t_id, ACE_hthread_t,
                  ACE_UINT32,
                  int grp_id,
                  ACE_Task_Base *task = 0,
                  long flags = 0,
                  ACE_Thread_Descriptor *td = 0);

  /// Remove thread from the table.
  void remove_thr (ACE_Thread_Descriptor *td,
                   int close_handler);

  /// Remove all threads from the table.
  void remove_thr_all (void);

  // = The following four methods implement a simple scheme for
  // operating on a collection of threads atomically.

  /**
   * Efficiently check whether <thread> is in a particular <state>.
   * This call updates the TSS cache if possible to speed up
   * subsequent searches.
   */
  int check_state (ACE_UINT32 state,
                   ACE_thread_t thread,
                   int enable = 1);

  /// Apply <func> to all members of the table that match the <task>
  int apply_task (ACE_Task_Base *task,
                  ACE_THR_MEMBER_FUNC,
                  int = 0);

  /// Apply <func> to all members of the table that match the <grp_id>.
  int apply_grp (int grp_id,
                 ACE_THR_MEMBER_FUNC func,
                 int arg = 0);

  /// Apply <func> to all members of the table.
  int apply_all (ACE_THR_MEMBER_FUNC,
                 int  = 0);

  /// Join the thread described in <tda>.
  int join_thr (ACE_Thread_Descriptor *td,
                int = 0);

  /// Resume the thread described in <tda>.
  int resume_thr (ACE_Thread_Descriptor *td,
                  int = 0);

  /// Suspend the thread described in <tda>.
  int suspend_thr (ACE_Thread_Descriptor *td,
                   int = 0);

  /// Send signal <signum> to the thread described in <tda>.
  int kill_thr (ACE_Thread_Descriptor *td,
                int signum);

  /// Set the cancellation flag for the thread described in <tda>.
  int cancel_thr (ACE_Thread_Descriptor *td,
                  int async_cancel = 0);

  /// Register a thread as terminated and put it into the <terminated_thr_list_>.
  int register_as_terminated (ACE_Thread_Descriptor *td);

  /**
   * Keeping a list of thread descriptors within the thread manager.
   * Double-linked list enables us to cache the entries in TSS
   * and adding/removing thread descriptor entries without
   * affecting other thread's descriptor entries.
   */
  ACE_Double_Linked_List<ACE_Thread_Descriptor> thr_list_;

#if !defined (VXWORKS)
  /// Collect terminated but not yet joined thread entries.
  ACE_Double_Linked_List<ACE_Thread_Descriptor_Base> terminated_thr_list_;
#endif /* VXWORKS */

  /// Collect pointers to thread descriptors of threads to be removed later.
  ACE_Unbounded_Queue<ACE_Thread_Descriptor*> thr_to_be_removed_;

  /// Keeps track of the next group id to assign.
  int grp_id_;

  /// Set if we want the Thread_Manager to wait on all threads before
  /// being closed, reset otherwise.
  int automatic_wait_;

  // = ACE_Thread_Mutex and condition variable for synchronizing termination.
#if defined (ACE_HAS_THREADS)
  /// Serialize access to the <zero_cond_>.
  ACE_Thread_Mutex lock_;

  /// Keep track of when there are no more threads.
  ACE_Condition_Thread_Mutex zero_cond_;
#endif /* ACE_HAS_THREADS */

private:
  ACE_Locked_Free_List<ACE_Thread_Descriptor, ACE_SYNCH_MUTEX> thread_desc_freelist_;

#if ! defined (ACE_THREAD_MANAGER_LACKS_STATICS)
  /// Pointer to a process-wide <ACE_Thread_Manager>.
  static ACE_Thread_Manager *thr_mgr_;

  /// Must delete the <thr_mgr_> if non-0.
  static int delete_thr_mgr_;
#endif /* ! defined (ACE_THREAD_MANAGER_LACKS_STATICS) */
};

#if defined (ACE_THREAD_MANAGER_LACKS_STATICS)
#define ACE_THREAD_MANAGER_SINGLETON_DEFINE \
        ACE_Singleton<ACE_Thread_Manager, ACE_SYNCH_MUTEX>;
typedef ACE_Singleton<ACE_Thread_Manager, ACE_SYNCH_MUTEX> ACE_THREAD_MANAGER_SINGLETON;
#endif /* defined (ACE_THREAD_MANAGER_LACKS_STATICS) */

#if defined (__ACE_INLINE__)
#include "ace/Thread_Manager.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_THREAD_MANAGER_H */
