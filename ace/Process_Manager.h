/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Process_Manager.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_PROCESS_MANAGER_H
#define ACE_PROCESS_MANAGER_H
#include /**/ "ace/pre.h"

#include "ace/Synch.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Process.h"

/**
 * @class ACE_Process_Descriptor
 *
 * @brief Information describing each process that's controlled by an
 * <ACE_Process_Manager>.
 */
class ACE_Export ACE_Process_Descriptor
{
private:
  friend class ACE_Process_Manager;

  /// Default ctor/dtor.
  ACE_Process_Descriptor (void);
  ~ACE_Process_Descriptor (void);

  /// Describes the process itself.
  ACE_Process *process_;

  /// function to call when process exits
  ACE_Event_Handler *exit_notify_;

  /// Dump the state of an object.
  void dump (void) const;
};

/**
 * @class ACE_Process_Manager
 *
 * @brief Manages a group of processes.
 *
 * This class allows applications to control groups of processes,
 * similar to how the <ACE_Thread_Manager> controls groups of
 * threads.  Naturally, it doesn't work at all on platforms, such
 * as VxWorks or pSoS, that don't support process.
 * There are two (main) ways of using <ACE_Process_Manager>,
 * depending on how involved you wish to be with the termination
 * of managed <ACE_Process>es.  If you just want <Process>es to
 * go away when they're finished, simply register the
 * <Process_Manager> with an <ACE_Reactor>:
 * ACE_Process_Manager mgr( 100, some_reactor )
 * -or-
 * ACE_Process_Manager mgr;
 * ...
 * mgr.open( 100, some_reactor );
 * Then, the <Process_Manager> will clean up after any
 * <Process>es that it spawns.  (On Unix, this means executing a
 * wait(2) to collect the exit status -- and avoid zombie
 * processes; on Win32, it means closing the process and thread
 * HANDLEs that are created when CreateProcess is called.)
 * If, on the other hand (and for some inexplicable reason) you
 * want to explicitly invoke the terminated <Process> cleanup
 * code, then *don't* register the <Process_Manager> with a
 * Reactor, and be sure to call one of the
 * <Process_Manager::wait> functions whenever there might be
 * managed <Process>es that have exited.
 * Note that in either case, <Process_Manager> allows you to
 * register "<Event_Handlers>" to be called when a specific
 * <Process> exits, or when any <Process> without a specific
 * <Event_Handler> exits.  When a <Process> exits, the
 * appropriate <Event_Handler>'s <handle_input> is called; the
 * <ACE_HANDLE> passed is either the Process' HANDLE (on Win32),
 * or its pid cast to an <ACE_HANDLE> (on unix).
 * It is also possible to call the <Process_Manager::wait>
 * functions even though the <Process_Manager> is registered with
 * a <Reactor>.
 * Note also that the wait functions are "sloppy" on Unix,
 * because there's no good way to wait for a subset of the
 * children of a process.  The wait functions may end up
 * collecting the exit status of a process that's not managed by
 * the <Process_Manager> whose <wait> you invoked.  It's best to
 * only use a single <Process_Manager>, and to create all
 * subprocesses by calling that <Process_Manager>'s <spawn>
 * method.  
 * Incidentally, when you register your <Process_Manager> with a
 * <Reactor> its notification pipe is used to help "reap" the
 * available exit statuses.  Therefore, you must not use a
 * <Reactor> whose notify pipe has been disabled.  Here's the
 * sequence of steps used to reap the exit statuses in this case:
 * + The <Process_Manager> registers a signal handler for
 *   SIGCHLD.
 * + The SIGCHLD handler, when invoked, uses the <Reactor>'s
 *   <notify> method to inform the <Reactor> to wake up.
 * + Next, the <Reactor> calls the <Process_Manager>'s
 *   <handle_input>, this happens synchronously, not in
 *   sighandler-space.
 * + The <handle_input> method collects all available exit
 *   statuses.
 */
class ACE_Export ACE_Process_Manager : protected ACE_Event_Handler
{
public:
  friend class ACE_Process_Control;

  enum
  {
    DEFAULT_SIZE = 100
  };

  // = Initialization and termination methods.
  /**
   * Initialize an <ACE_Process_Manager> with a table containing up to
   * <size> processes.  This table resizes itself automatically as
   * needed.  If a non-NULL <reactor> is provided, this
   * <ACE_Process_Manager> uses it to notify an application when a
   * process it controls exits.  By default, however, we don't use an
   * <ACE_Reactor>.
   */
  ACE_Process_Manager (size_t size = ACE_Process_Manager::DEFAULT_SIZE,
                       ACE_Reactor *reactor = 0);

  /**
   * Initialize an <ACE_Process_Manager> with a table containing up to
   * <size> processes.  This table resizes itself automatically as
   * needed.  If a non-NULL <reactor> is provided, this
   * <ACE_Process_Manager> uses it to notify an application when a
   * process it controls exits.  By default, however, we don't use an
   * <ACE_Reactor>.
   */
  int open (size_t size = DEFAULT_SIZE,
            ACE_Reactor *r = 0);

  /// Release all resources.  Do not wait for processes to exit.
  int close (void);

  /// Destructor releases all resources and does not wait for processes
  /// to exit.
  virtual ~ACE_Process_Manager (void);

  // = Singleton accessors.
  /// Get pointer to a process-wide <ACE_Process_Manager>.
  static ACE_Process_Manager *instance (void);

  /// Set pointer to a process-wide <ACE_Process_Manager> and return
  /// existing pointer.
  static ACE_Process_Manager *instance (ACE_Process_Manager *);

  /// Delete the dynamically allocated singleton.
  static void close_singleton (void);

  /// Cleanup method, used by the <ACE_Object_Manager> to destroy the
  /// singleton.
  static void cleanup (void *instance, void *arg);

  // = Process creation methods.

  /**
   * Create a new process by passing <options> to <proc.spawn>.  On
   * success, returns the process id of the child that was created.
   * On failure, returns ACE_INVALID_PID.
   */
  pid_t spawn (ACE_Process *proc,
               ACE_Process_Options &options);

  /**
   * Create a new process by passing <options> to
   * <ACE_Process::spawn>.  On success, returns the process id of the
   * child that was created.  On failure, returns ACE_INVALID_PID.
   */
  pid_t spawn (ACE_Process_Options &options);

  /**
   * Create <n> new processes by passing <options> to
   * <ACE_Process::spawn>, which is called <n> times.  If <child_pids>
   * is non-0 it is expected to be an array of <n> <pid_t>'s, which
   * are filled in with the process ids of each newly created process.
   * Returns 0 on success and -1 on failure.
   */
  int spawn_n (size_t n,
               ACE_Process_Options &options,
               pid_t *child_pids = 0);

  // = Process synchronization operations.

  /**
   * Block until there are no more child processes running that were
   * <spawn>ed by this <ACE_Process_Manager>.  Unlike the <wait> call
   * below, this method does not require a signal handler or
   * <ACE_OS::sigwait> because it simply blocks synchronously waiting
   * for all the children managed by this <ACE_Process_Manager> to
   * exit.  Note that this does not return any status information
   * about the success or failure of exiting child processes, although
   * any registered exit_handlers are called.  Returns 0 on success
   * (and <remove>s the corresponding <ACE_Process_Descriptor> entries
   * from the <Process_Manager>; otherwise, returns -1 on failure.
   */
  int wait (const ACE_Time_Value &timeout = ACE_Time_Value::max_time);

  /**
   * Wait up to <timeout> for a single process to terminate.  If
   * pid==0, waits for any of the managed <Process>es (but see the
   * note in the class documentation above for caveats about this --
   * "sloppy process cleanup on unix") If pid != 0, waits for that <Process>
   * only.  Returns the pid of the Process whose exit was handled, 0
   * if a timeout occurred, or ACE_INVALID_PID on error.
   */
  pid_t wait (pid_t pid,
              const ACE_Time_Value &timeout,
              ACE_exitcode *status = 0);

  /**
   * Wait indefinitely for a single process to terminate.  If pid==0,
   * waits for any of the managed <Process>es (but see the note in
   * the class documentation for caveats about this -- "sloppy Process
   * cleanup on unix") If pid != 0, waits for that <Process> only.
   * Returns the pid of the process whose exit was handled, or
   * ACE_INVALID_PID on error.
   */
  pid_t wait (pid_t pid,
              ACE_exitcode *status = 0);

  /**
   * Reap the result of a single process by calling <ACE_OS::waitpid>,
   * therefore, this method is not portable to Win32.  If the child is
   * successfully reaped, <remove> is called automatically.  This
   * method does the same thing that the <wait> method directly above
   * it does -- It's just here for backwards compatibility.
   */
  int reap (pid_t pid = -1,
            ACE_exitcode *stat_loc = 0,
            int options = WNOHANG);

  // = Utility methods.
  /**
   * Register an Event_Handler to be called back when the specified
   * process exits.  If pid == ACE_INVALID_PID this handler is called
   * when any process with no specific handler exits.
   */
  int register_handler (ACE_Event_Handler *event_handler,
                        pid_t pid = ACE_INVALID_PID);

  /**
   * Remove process <pid> from the table.  This is called
   * automatically by the <reap> method after it successfully reaped a
   * <SIGCHLD> signal.  It's also possible to call this method
   * directly from a signal handler, but don't call both <reap> and
   * <remove>!
   */
  int remove (pid_t pid);

  /**
   * Abruptly terminate a single process with id <pid> using the
   * <ACE::terminate_process> method.  Note that this call is
   * potentially dangerous to use since the process being terminated
   * may not have a chance to cleanup before it shuts down.  Returns 0
   * on success and -1 on failure.
   */
  int terminate (pid_t pid);

  /// On OSs that support signals, send the signal to the specified
  /// process.  Returns 0 on success and -1 on failure.
  int terminate (pid_t pid,
                 int sig);

  /// Return the number of managed Processes.
  size_t managed (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  // = These methods allow a <Process_Manager> to be an <Event_Handler>.

  // As an <Event_Handler>, the <Process_Manager> automagically
  // detects child Processes exiting and calls notify_proc_handler()
  // and remove().  This means that you don't have to (shouldn't!)
  // call the wait(...)  methods yourself.

  // On Unix, we can't detect individual process termination very
  // well; the best method is to catch SIGCHLD and then call the
  // polling wait() function to collect any available exit statuses.
  // However, we don't want to do this from within a signal handler
  // because of the restrictions associated.  Therefore (following the
  // lead in examples/mumble) we open a bogus handle (to ACE_DEV_NULL)
  // and register that handle with our Reactor.  Then, when our
  // SIGCHLD handler gets invoked, we tell the Reactor that the bogus
  // handle is readable.  That will cause the handle_input() function
  // to be called once we're out of the interrupt context, and
  // handle_input() collects exit statuses.

  // On Win32, we simply register ourself with the Reactor to deal
  // with the Process handle becoming signaled.  No muss, no fuss, no
  // signal handler, and no dummy handle.

#if !defined(ACE_WIN32)
  /// Collect one (or more, on unix) process exit status.
  virtual int handle_input (ACE_HANDLE proc);
#endif // !defined(ACE_WIN32)

  /**
   * On Unix, this routine is called asynchronously when a SIGCHLD is
   * received.  We just tweak the reactor so that it'll call back our
   * <handle_input> function, which allows us to handle Process exits
   * synchronously.
   *
   * On Win32, this routine is called synchronously, and is passed the
   * HANDLE of the Process that exited, so we can do all our work here
   */
  virtual int handle_signal (int signum,
                             siginfo_t * = 0,
                             ucontext_t * = 0);

private:
  /// Resize the pool of Process_Descriptors.
  int resize (size_t);

  /// Locate the index of the table slot occupied by <process_id>.
  /// Returns -1 if <process_id> is not in the <process_table_>
  ssize_t find_proc (pid_t process_id);

#if defined (ACE_WIN32)
  /// Locate the index of the table slot occupied by <process_handle>.
  /// Returns ~0 if <process_handle> is not in the <process_table_>
  ssize_t find_proc (ACE_HANDLE process_handle);
#endif /* ACE_WIN32 */

  /// Insert a process in the table (checks for duplicates).  Omitting
  /// the process handle won't work on Win32...
  int insert_proc (ACE_Process *process);

  /**
   * Append information about a process, i.e., its <process_id> in the
   * <process_table_>.  Each entry is added at the end, growing the
   * table if necessary.
   */
  int append_proc (ACE_Process *process);

  /// Actually removes the process at index <n> from the table.  This method
  /// must be called with locks held.
  int remove_proc (size_t n);

  /// If there's a specific handler for the Process at index <n> in the
  /// table, or there's a default handler, call it.
  int notify_proc_handler (size_t n,
                           ACE_exitcode status);

  /// Vector that describes process state within the Process_Manager.
  ACE_Process_Descriptor *process_table_;

  /// Maximum number of processes we can manage (should be dynamically
  /// allocated).
  size_t max_process_table_size_;

  /// Current number of processes we are managing.
  size_t current_count_;

  /// This event handler is used to notify when a process we control
  /// exits.
  ACE_Event_Handler *default_exit_handler_;

  /// Singleton pointer.
  static ACE_Process_Manager *instance_;

  /// Controls whether the <Process_Manager> is deleted when we shut
  /// down (we can only delete it safely if we created it!)
  static int delete_instance_;

#if defined (ACE_HAS_THREADS)
  /// This lock protects access/ops on <process_table_>.
  ACE_Recursive_Thread_Mutex lock_;
#endif /* ACE_HAS_THREADS */
};

#if defined (__ACE_INLINE__)
#include "ace/Process_Manager.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_PROCESS_MANAGER_H */
