/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Process_Manager.h
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_PROCESS_MANAGER_H
#define ACE_PROCESS_MANAGER_H

#include "ace/Synch.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Process.h"

class ACE_Export ACE_Process_Descriptor
{
  // = TITLE
  //    Information describing each process that's controlled by an
  //    <ACE_Process_Manager>.
private:
  friend class ACE_Process_Manager;

  ACE_Process_Descriptor (void);
  ~ACE_Process_Descriptor (void);
  // Default ctor/dtor.

  int delete_process_;
  // Keeps track of whether we need to delete the Process or just
  // <close> it?

  ACE_Process *process_;
  // Describes the process itself.

  ACE_Event_Handler *exit_notify_;
  // function to call when this process exits.

  void dump (void) const;
  // Dump the state of an object.
};

class ACE_Export ACE_Process_Manager : protected ACE_Event_Handler
{
  // = TITLE
  //    Manages a group of processes.
  //
  // = DESCRIPTION
  //    This class allows applications to control groups of processes,
  //    similar to how the <ACE_Thread_Manager> controls groups of
  //    threads.  Naturally, it doesn't work at all on platforms, such
  //    as VxWorks or pSoS, that don't support process.  Moreover,
  //    it's best to avoid the <wait> methods in this class that
  //    require the use of signals since these aren't portable to
  //    Win32.  In addition, if you choose to use signals, try to use
  //    the <ACE_OS::sigwait> variant of <{synchronous}> signal
  //    handling, rather than <{asynchronous}> signal handling.  See
  //    $ACE_ROOT/tests/Process_Manager_Test.cpp for an illustrate of
  //    how to use <ACE_OS::sigwait> in conjunction with the
  //    <ACE_Process_Manager>.
public:
  friend class ACE_Process_Control;

  enum
  {
    DEFAULT_SIZE = 100
  };

  // = Initialization and termination methods.
  ACE_Process_Manager (size_t size = ACE_Process_Manager::DEFAULT_SIZE,
                       ACE_Reactor *reactor = 0);
  // Initialize an <ACE_Process_Manager> with a table containing up to
  // <size> processes.  This table resizes itself automatically as
  // needed.  If a non-NULL <reactor> is provided, this
  // <ACE_Process_Manager> uses it to notify an application when a
  // process it controls exits.  By default, however, we don't use an
  // <ACE_Reactor>.

  int open (size_t size = DEFAULT_SIZE,
            ACE_Reactor *r = ACE_Reactor::instance ());
  // Initialize an <ACE_Process_Manager> with a table containing up to
  // <size> processes.  This table resizes itself automatically as
  // needed.  If a non-NULL <reactor> is provided, this
  // <ACE_Process_Manager> uses it to notify an application when a
  // process it controls exits.  By default, however, we don't use an
  // <ACE_Reactor>.

  int close (void);
  // Release all resources.  Do not wait for processes to exit.

  virtual ~ACE_Process_Manager (void);
  // Destructor releases all resources and does not wait for processes
  // to exit.

  // = Singleton accessors.
  static ACE_Process_Manager *instance (void);
  // Get pointer to a process-wide <ACE_Process_Manager>.

  static ACE_Process_Manager *instance (ACE_Process_Manager *);
  // Set pointer to a process-wide <ACE_Process_Manager> and return
  // existing pointer.

  // = Process creation methods.

  pid_t spawn (ACE_Process *proc,
               ACE_Process_Options &options);
  // Create a new process by passing <options> to <proc.spawn>.  On
  // success, returns the process id of the child that was created.
  // On failure, returns -1.

  pid_t spawn (ACE_Process_Options &options);
  // Create a new process by passing <options> to
  // <ACE_Process::spawn>.  On success, returns the process id of the
  // child that was created.  On failure, returns -1.

  int spawn_n (size_t n,
               ACE_Process_Options &options,
               pid_t *child_pids = 0);
  // Create <n> new processes by passing <options> to
  // <ACE_Process::spawn>, which is called <n> times.  If <child_pids>
  // is non-0 it is expected to be an array of <n> <pid_t>'s, which
  // are filled in with the process ids of each newly created process.
  // Returns 0 on success and -1 on failure.

  // = Process synchronization operations.

  int wait (void);
  // Block until there are no more child processes running that were
  // <spawn>ed by this <ACE_Process_Manager>.  Unlike the <wait> call
  // below, this method does not require a signal handler or
  // <ACE_OS::sigwait> because it simply blocks synchronously waiting
  // for all the children managed by this <ACE_Process_Manager> to
  // exit.  Note that this does not return any status information
  // about the success or failure of exiting child processes, although
  // any registered exit_handlers are called.  Returns
  // 0 on success (and <remove>s the corresponding
  // <ACE_Process_Descriptor> entries from the <Process_Manager>;
  // otherwise, returns -1 on failure.

  int wait (ACE_Time_Value *timeout);
  // Block until there are no more child processes running that were
  // <spawn>ed by this <ACE_Process_Manager> or <timeout> expires.
  // Returns 0 on success and -1 on failure.

  int wait (pid_t pid, ACE_Time_Value *timeout);
  // Block until pid exits or <timeout> expires.
  // Returns 0 on success and -1 on failure.

  int wait (pid_t pid = -(ACE_OS::getpid( )),
            int *stat_loc = 0,
            int options = WNOHANG );
  // Reap the result of a single process by calling <ACE_OS::wait>.
  // If the child is successfully reaped, <remove> is called
  // automatically.  Note that this method can be portably called
  // within an asynchronous signal handler only if the platform allows
  // signal handlers to <acquire>/<release> threads.  This method
  // works portably on platforms that support <ACE_OS::sigwait> -- in
  // general, it's better to use <ACE_OS::sigwait> to wait for signals
  // synchronously rather than asynchronously anyhow.

  int reap (pid_t pid = -1,
            int *stat_loc = 0,
            int options = WNOHANG);
  // Reap the result of a single process by calling <ACE_OS::waitpid>,
  // therefore, this method is not portable to Win32.  If the child is
  // successfully reaped, <remove> is called automatically.  This
  // method does the same thing that the <wait> method directly above
  // it does -- It's just here for backwards compatibility.

  // = Utility methods.
  int register_handler (ACE_Event_Handler *event_handler,
                        pid_t pid = ACE_INVALID_PID);
  // Register an Event_Handler to be called back when the specified
  // process exits.  If pid == ACE_INVALID_PID this handler is called
  // when any process with no specific handler exits.
	
  int remove (pid_t pid);
  // Remove process <pid> from the table.  This is called
  // automatically by the <reap> method after it successfully reaped a
  // <SIGCHLD> signal.  It's also possible to call this method
  // directly from a signal handler, but don't call both <reap> and
  // <remove>!

  int terminate (pid_t pid);
  // Abruptly terminate a single process with id <pid> using the
  // <ACE::terminate_process> method.  Note that this call is
  // potentially dangerous to use since the process being terminated
  // may not have a chance to cleanup before it shuts down.  Returns 0
  // on success and -1 on failure.

  int terminate (pid_t pid, int sig);
  // On OSs that support signals, send the signal to the specified
  // process.  Returns 0 on success and -1 on failure.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = These methods allow a <Process_Manager> to be an <Event_Handler>.

  // @@ Dave, can you please add comments for these methods.
  virtual ACE_HANDLE get_handle (void) const;
  // Return the handle.

  virtual int handle_input (ACE_HANDLE);
  // 
  virtual int handle_signal (int signum, siginfo_t * = 0,
                             ucontext_t * = 0);
  // 
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);
  // 

private:
  int resize (size_t);
  // Resize the pool of Process_Descriptors.

  int find_proc (pid_t process_id);
  // Locate the index of the table slot occupied by <process_id>.
  // Returns -1 if <process_id> is not in the <process_table_>

#if defined (ACE_WIN32)
  int find_proc (ACE_HANDLE process_handle);
  // Locate the index of the table slot occupied by <process_handle>.
  // Returns -1 if <process_handle> is not in the <process_table_>
#endif /* ACE_WIN32 */

  int insert_proc (ACE_Process *process);
  // Insert a process in the table (checks for duplicates).  Omitting
  // the process handle won't work on Win32...

  int append_proc (ACE_Process *process);
  // Append information about a process, i.e., its <process_id> in the
  // <process_table_>.  Each entry is added at the end, growing the
  // table if necessary.

  int remove_proc (pid_t pid);
  // Actually removes the process <pid> from the table.  This method
  // must be called with locks held.

  int notify_proc_handler (ACE_HANDLE proc,
                           pid_t pid,
                           int status);
  // If there's a specific handler for <pid>'s exit, or a default
  // handler, call it.

  ACE_Process_Descriptor *process_table_;
  // Vector that describes process state within the Process_Manager.

  size_t max_process_table_size_;
  // Maximum number of processes we can manage (should be dynamically
  // allocated).

  size_t current_count_;
  // Current number of processes we are managing.

  ACE_HANDLE dummy_handle_;
  // Allows SIGCHLD to be handled synchronously.

  ACE_Event_Handler *default_exit_handler_;
  // This event handler is used to notify when a process we control
  // exits.

  static ACE_Process_Manager *instance_;
  // Singleton pointer.

  static int delete_instance_;
  // Controls whether the <Process_Manager> is deleted when we shut
  // down (we can only delete it safely if we created it!)

#if defined (ACE_HAS_THREADS)
  // = ACE_Thread_Mutex and condition variable for synchronizing termination.
  ACE_Thread_Mutex lock_;
  ACE_Condition_Thread_Mutex zero_cond_;
#endif /* ACE_HAS_THREADS */
};

#if defined (__ACE_INLINE__)
#include "ace/Process_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_PROCESS_MANAGER_H */
