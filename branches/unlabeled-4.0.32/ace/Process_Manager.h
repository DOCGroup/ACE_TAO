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

#include "ace/Log_Msg.h"
#include "ace/Synch.h"

#if 0
class ACE_Export ACE_Process_Descriptor
  // = Title
  //    Information for controlling groups of processs.
{
friend class ACE_Process_Manager;
public:
  enum Process_State
  {
    IDLE,
    SPAWNED,
    RUNNING,
    SUSPENDED,
    TERMINATED
  };
private:

  ACE_Process_Descriptor (void);

  pid_t proc_id_;
  // Unique process ID.
    
  int grp_id_;
  // Unique group ID.

  Process_State proc_state_;
  // Current state of the process.
};

// Forward declaration.

class ACE_Process_Control;

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

  int spawn (ACE_THR_FUNC func, 
	     void *args, 
	     long flags, 
	     pid_t * = 0, 
	     u_int priority = 0,
	     void *stack = 0, 
	     size_t stack_size = 0);
  // Create a new process, which executes <func>.  

  // Returns: on success a unique group id that can be used to control
  // other processs added to the same group.  On failure, returns -1.

  int spawn_n (int n, 
	       ACE_THR_FUNC func, 
	       void *args, 
	       long flags,
	       u_int priority = 0);
  // Create N new processs, all of which execute <func>.  
  
  // Returns: on success a unique group id that can be used to control
  // all of the processs in the same group.  On failure, returns -1.

  void *exit (void *status);	
  // Called to clean up when a process exits.

  int wait (ACE_Time_Value *timeout = 0);	
  // Block until there are no more processs running or <timeout>
  // expires.  Returns 0 on success and -1 on failure.

  // = Suspend methods.
  int suspend_all (void);
  // Suspend all processs 
  int suspend (pid_t);
  // Suspend a single process.
  int suspend_grp (int grp_id);
  // Suspend a group of processs.

  // = Resume methods.
  int resume_all (void);
  // Resume all stopped processs 
  int resume (pid_t);
  // Resume a single process.
  int resume_grp (int grp_id);
  // Resume a group of processs.

  // = Kill methods (send signals...).
  int kill_all (int signum);
  // Send signum to all stopped processs 
  int kill (pid_t, int signum);
  // Kill a single process.
  int kill_grp (int grp_id, int signum);
  // Kill a group of processs.

  // = Set/get group ids for a particular process id.
  int set_grp (pid_t, int grp_id);
  int get_grp (pid_t, int &grp_id);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int resize (size_t);
  // Resize the pool of Process_Descriptors.

  int spawn_i (ACE_THR_FUNC func, 
	       void *args, 
	       long flags, 
	       pid_t * = 0, 
	       u_int priority = 0,
	       void *stack = 0, 
	       size_t stack_size = 0);
  // Create a new process (must be called with locks held).

  int find (pid_t p_id);
  // Locate the index of the table slot occupied by <p_id>.  Returns
  // -1 if <p_id> is not in the table doesn't contain <p_id>.

  int insert_proc (pid_t p_id);
  // Insert a process in the table (checks for duplicates).
  // Omitting the process handle won't work on Win32...

  int append_proc (pid_t p_id, ACE_Process_Descriptor::Process_State);
  // Append a process in the table (adds at the end, growing the table
  // if necessary).

  void remove_proc (int i);	
  // Remove process from the table. 

  // = The following four methods implement a simple scheme for
  // operating on a collection of processs atomically.

  typedef int (ACE_Process_Manager::*PROC_FUNC)(int, int);

  int apply_grp (int grp_id, PROC_FUNC, int = 0);
  // Apply <func> to all members of the table that match the <grp_id>.

  int apply_all (PROC_FUNC, int = 0);
  // Apply <func> to all members of the table.

  int resume_proc (int i);
  // Resume the process at index <i>.

  int suspend_proc (int i);
  // Suspend the process at index <i>.

  int kill_proc (int i, int signum);
  // Send signal <signum> to the process at index <i>.

  ACE_Process_Descriptor *proc_table_;
  // Vector that describes process state within the Process_Manager.

  size_t max_table_size_;
  // Maximum number of processs we can manage (should be dynamically
  // allocated).

  size_t current_count_;
  // Current number of processs we are managing.

  int grp_id_;
  // Keeps track of the next group id to assign.
};

class ACE_Export ACE_Process_Control
  // = TITLE
  //     Used to keep track of a process's activities within its entry
  //     point function.
{
public:
  ACE_Process_Control (ACE_Process_Manager *tm, int insert = 0);
  // Initialize the process control object.  If INSERT != 0, then
  // register the process with the ProcessManager.

  ~ACE_Process_Control (void);
  // Implicitly kill the process on exit and remove it from its
  // associated ProcessManager.

  void *exit (void *status);
  // Explicitly kill the process on exit and remove it from its
  // associated ProcessManager.

  void *status (void *status);
  // Set the exit status (and return existing status).

  void *status (void);
  // Get the current exit status.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Process_Manager *tm_;
  // Pointer to the process manager for this block of code.

  void *status_;
  // Keeps track of the exit status for the process.
};

#if defined (__ACE_INLINE__)
#include "ace/Process_Manager.i"
#endif /* __ACE_INLINE__ */
#endif

#endif /* ACE_PROCESS_MANAGER_H */

