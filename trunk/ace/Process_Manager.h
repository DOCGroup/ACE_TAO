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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Process.h"

class ACE_Export ACE_Process_Descriptor
{
  // = TITLE
  //    Information describing each process that's controlled by the
  //    <ACE_Process_Manager>.
private:
  friend class ACE_Process_Manager;

  ACE_Process_Descriptor (void);
  ~ACE_Process_Descriptor (void);
  // Default ctor/dtor.

  pid_t proc_id_;
  // Unique process ID.

  gid_t grp_id_;
  // Unique group ID.

  void dump (void) const;
  // Dump the state of an object.
};

class ACE_Export ACE_Process_Manager
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
  ACE_Process_Manager (size_t size = ACE_Process_Manager::DEFAULT_SIZE);
  virtual ~ACE_Process_Manager (void);

  static ACE_Process_Manager *instance (void);
  // Get pointer to a process-wide <ACE_Process_Manager>.

  static ACE_Process_Manager *instance (ACE_Process_Manager *);
  // Set pointer to a process-wide <ACE_Process_Manager> and return
  // existing pointer.

  int open (size_t size = DEFAULT_SIZE);
  // Initialize an <ACE_Process_Manager> with a table containing up to
  // <size> processes.  This table resizes itself automatically as
  // needed.

  int close (void);
  // Release all resources.

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

  int wait (void);
  // Block until there are no more child processes running that were
  // <spawn>ed by this <ACE_Process_Manager>.  Unlike the <wait> call
  // below, this method does not require a signal handler or
  // <ACE_OS::sigwait> because it simply blocks synchronously waiting
  // for all the children managed by this <ACE_Process_Manager> to
  // exit.  Note that this does not return any status information
  // about the success or failure of exiting child processes.  Returns
  // 0 on success (and <remove>s the corresponding
  // <ACE_Process_Descriptor> entry from the <Process_Manager>;
  // otherwise, returns -1 on failure.

  int wait (ACE_Time_Value *timeout);
  // Block until there are no more child processes running that were
  // <spawn>ed by this <ACE_Process_Manager> or <timeout> expires.  In
  // order for this <wait> method to work you'll need to register a
  // signal handler or use <ACE_OS::sigwait> to call the <reap> or
  // <remove> method when a <SIGCHILD> signal occurs.  Therefore, this
  // method is not portable to Win32...  Returns 0 on success and -1
  // on failure.

  int wait (pid_t pid,
            int *stat_loc,
            int options);
  // Reap the result of a single process by calling <ACE_OS::waitpid>,
  // therefore, this method is not portable to Win32.  If the child is
  // successfully reaped, <remove> is called automatically.  Note that
  // this method can be portably called within an asynchronous signal
  // handler only if the platform allows signal handlers to
  // <acquire>/<release> threads.  This method works portably on
  // platforms that support <ACE_OS::sigwait> -- in general, it's
  // better to use <ACE_OS::sigwait> to wait for signals synchronously
  // rather than asynchronously anyhow.

  int reap (pid_t pid = -1,
            int *stat_loc = 0,
            int options = WNOHANG);
  // Reap the result of a single process by calling <ACE_OS::waitpid>,
  // therefore, this method is not portable to Win32.  If the child is
  // successfully reaped, <remove> is called automatically.  This
  // method does the same thing that the <wait> method directly above
  // it does -- It's just here for backwards compatibility.

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

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int resize (size_t);
  // Resize the pool of Process_Descriptors.

  int find_proc (pid_t process_id);
  // Locate the index of the table slot occupied by <process_id>.
  // Returns -1 if <process_id> is not in the <process_table_> doesn't
  // contain <process_id>.

  int insert_proc (pid_t process_id);
  // Insert a process in the table (checks for duplicates).
  // Omitting the process handle won't work on Win32...

  int append_proc (pid_t process_id);
  // Append information about a process, i.e., its <process_id> in the
  // <process_table_>.  Each entry is added at the end, growing the
  // table if necessary.

  int remove_proc (pid_t pid);
  // Actually removes the process <pid> from the table.  This method
  // must be called with locks held.

  ACE_Process_Descriptor *process_table_;
  // Vector that describes process state within the Process_Manager.

  size_t max_process_table_size_;
  // Maximum number of processes we can manage (should be dynamically
  // allocated).

  size_t current_count_;
  // Current number of processes we are managing.

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

