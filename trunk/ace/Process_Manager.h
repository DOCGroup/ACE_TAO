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
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_PROCESS_MANAGER_H)
#define ACE_PROCESS_MANAGER_H

#include "ace/Synch.h"
#include "ace/Process.h"

class ACE_Export ACE_Process_Descriptor
  // = Title
  //    Information for controlling groups of processs.
{
friend class ACE_Process_Manager;
private:

  ACE_Process_Descriptor (void);

  pid_t proc_id_;
  // Unique process ID.
    
  gid_t grp_id_;
  // Unique group ID.

  void dump (void) const;
  // Dump the state of an object.
};

class ACE_Export ACE_Process_Manager
  // = TITLE
  //    Manages a pool of processs.
  //
  // = DESCRIPTION
  //    This class allows operations on groups of processs atomically. 
{
friend class ACE_Process_Control;
public:
  enum
  {
    DEFAULT_SIZE = 100
  };

  // = Initialization and termination methods.
  ACE_Process_Manager (size_t size = ACE_Process_Manager::DEFAULT_SIZE);
  ~ACE_Process_Manager (void);

  int open (size_t size = DEFAULT_SIZE); 
  // Initialize the manager with room for SIZE processs.

  int close (void);		
  // Release all resources.

  pid_t start (ACE_Process_Options &options);
  // Create a new process using ACE_Process::start (<options>).

  // Returns: on success a unique group id that can be used to control
  // other processs added to the same group.  On failure, returns -1.

  int start_n (size_t n, ACE_Process_Options &options);
  // Create N new processs.
  
  // Returns: on success a unique group id that can be used to control
  // all of the processs in the same group.  On failure, returns -1.

  int wait (ACE_Time_Value *timeout = 0);	
  // Block until there are no more processs running or <timeout>
  // expires.  Returns 0 on success and -1 on failure.

  int kill (pid_t, int signum);
  // Kill a single process.

  int remove (pid_t pid);
  // Remove process <pid> from the table.  This is typically called by
  // a signal handler that has just reaped <SIGCHILD>.

  int reap (pid_t pid = -1, int *stat_loc = 0, int options = WNOHANG);
  // Reap a <SIGCHLD> by calling <ACE_OS::waitpid>.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int resize (size_t);
  // Resize the pool of Process_Descriptors.

  int find_proc (pid_t p_id);
  // Locate the index of the table slot occupied by <p_id>.  Returns
  // -1 if <p_id> is not in the table doesn't contain <p_id>.

  int insert_proc (pid_t p_id);
  // Insert a process in the table (checks for duplicates).
  // Omitting the process handle won't work on Win32...

  int append_proc (pid_t p_id);
  // Append a process in the table (adds at the end, growing the table
  // if necessary).

  ACE_Process_Descriptor *proc_table_;
  // Vector that describes process state within the Process_Manager.

  size_t max_table_size_;
  // Maximum number of processs we can manage (should be dynamically
  // allocated).

  size_t current_count_;
  // Current number of processs we are managing.

  // = ACE_Thread_Mutex and condition variable for synchronizing termination.
#if defined (ACE_HAS_THREADS)
  ACE_Thread_Mutex lock_;
  ACE_Condition_Thread_Mutex zero_cond_;
#endif /* ACE_HAS_THREADS */
};

#if defined (__ACE_INLINE__)
#include "ace/Process_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_PROCESS_MANAGER_H */

