// $Id$

// Process_Manager.cpp
#define ACE_BUILD_DLL
#include "ace/Process.h"
#include "ace/Process_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Process_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Process_Control)
ACE_ALLOC_HOOK_DEFINE(ACE_Process_Manager)

void
ACE_Process_Manager::dump (void) const
{
  ACE_TRACE ("ACE_Process_Manager::dump");
}

ACE_Process_Descriptor::ACE_Process_Descriptor (void)
{
  ACE_TRACE ("ACE_Process_Descriptor::ACE_Process_Descriptor");
}

int
ACE_Process_Manager::resize (size_t size)
{
  ACE_TRACE ("ACE_Process_Manager::resize");

  ACE_TRACE ("ACE_Thread_Manager::resize");
  ACE_Process_Descriptor *temp;
  
  ACE_NEW_RETURN (temp, ACE_Process_Descriptor[size], -1);

  for (size_t i = 0; i < this->max_table_size_; i++)
    temp[i] = this->proc_table_[i]; // Structure assignment.

  this->max_table_size_ = size;

  delete [] this->proc_table_;

  this->proc_table_ = temp;
  return 0;
}

// Create and initialize the table to keep track of the process pool.

int
ACE_Process_Manager::open (size_t size)
{
  ACE_TRACE ("ACE_Process_Manager::open");

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->max_table_size_ < size)
    this->resize (size);
  return 0;

}

// Initialize the synchronization variables.

ACE_Process_Manager::ACE_Process_Manager (size_t size)
  : proc_table_ (0),
    max_table_size_ (0), 
    current_count_ (0)
#if defined (ACE_HAS_THREADS)
    , zero_cond_ (lock_)
#endif /* ACE_HAS_THREADS */
{
  ACE_TRACE ("ACE_Process_Manager::ACE_Process_Manager");

  if (this->open (size) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Process_Manager"));
}

// Close up and release all resources.

int
ACE_Process_Manager::close (void)
{
  ACE_TRACE ("ACE_Process_Manager::close");

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->proc_table_ != 0)
    {
      delete [] this->proc_table_;
      this->proc_table_ = 0;
      this->max_table_size_ = 0;
      this->current_count_ = 0;
    }
  return 0;
}

ACE_Process_Manager::~ACE_Process_Manager (void)
{
  ACE_TRACE ("ACE_Process_Manager::~ACE_Process_Manager");
  this->close ();
}

// Create a new process running FUNC.  *Must* be called with the lock_
// held...

pid_t
ACE_Process_Manager::start (char *argv[],
			    char *envp[])
{
  ACE_TRACE ("ACE_Process_Manager::start");

  // Create a new process, potentially causing an exec(), as well.
  // This API clearly needs to be improved to pass more information
  // in...
  pid_t pid = ACE_Process process (argv,
				   ACE_INVALID_HANDLE,
				   ACE_INVALID_HANDLE,
				   ACE_INVALID_HANDLE,
				   envp);
  if (pid == -1)
    return -1;
  else
    return this->append_proc (pid);
}

// Create N new processs running FUNC.

int 
ACE_Process_Manager::start_n (size_t n,
			      char *argv[],
			      char *envp[])
{
  ACE_TRACE ("ACE_Process_Manager::spawn_n");

  for (size_t i = 0; i < n; i++)
    if (this->start (argv, envp) == -1)
      return -1;

  return 0;

}

// Append a process into the pool (does not check for duplicates).
// Must be called with locks held.

int
ACE_Process_Manager::append_proc (pid_t pid,
				  ACE_Process_Descriptor::Process_State proc_state)
{
  ACE_TRACE ("ACE_Process_Manager::append_proc");

  // Try to resize the array to twice its existing size if we run out
  // of space...
  if (this->current_count_ >= this->max_table_size_ 
      && this->resize (this->max_table_size_ * 2) == -1)
    return -1;
  else
    {
      ACE_Thread_Descriptor &proc_desc = 
	this->proc_table_[this->current_count_];

      proc_desc.proc_id_ = pid;
      proc_desc.proc_id_ = 
      proc_desc.proc_state_ = proc_state;

      this->current_count_++;
      return 0;
    }
}

// Insert a process into the pool (checks for duplicates and doesn't
// allow them to be inserted twice).

int
ACE_Process_Manager::insert_proc (pid_t t_id)
{
  ACE_TRACE ("ACE_Process_Manager::insert_proc");
  return -1;
}

// Remove a process from the pool.  Must be called with locks held.

void
ACE_Process_Manager::remove_proc (int i)
{
  ACE_TRACE ("ACE_Process_Manager::remove_proc");
}

int
ACE_Process_Manager::resume_proc (int i)
{
  ACE_TRACE ("ACE_Process_Manager::resume_proc");
  return -1;
}

int
ACE_Process_Manager::suspend_proc (int i)
{
  ACE_TRACE ("ACE_Process_Manager::suspend_proc");

  return -1;
}

int
ACE_Process_Manager::kill_proc (int i, int signum)
{
  ACE_TRACE ("ACE_Process_Manager::kill_proc");

  return -1;
}

// Locate the index in the table associated with <t_id>.  Must be
// called with the lock held.

int 
ACE_Process_Manager::find (pid_t t_id)
{
  ACE_TRACE ("ACE_Process_Manager::find");
  return -1;    
}

// Suspend a single process.

int 
ACE_Process_Manager::suspend (pid_t t_id)
{
  ACE_TRACE ("ACE_Process_Manager::suspend");
  return -1;
}

// Resume a single process.

int 
ACE_Process_Manager::resume (pid_t t_id)
{
  ACE_TRACE ("ACE_Process_Manager::resume");
  return -1;
}

// Kill a single process.
int 
ACE_Process_Manager::kill (pid_t t_id, int signum)
{
  ACE_TRACE ("ACE_Process_Manager::kill");
  return -1;
}

// Get group ids for a particular process id.

int 
ACE_Process_Manager::get_grp (pid_t t_id, int &grp_id)
{
  ACE_TRACE ("ACE_Process_Manager::get_grp");
  return -1;
}

// Set group ids for a particular process id.

int 
ACE_Process_Manager::set_grp (pid_t t_id, int grp_id)
{
  ACE_TRACE ("ACE_Process_Manager::set_grp");
  return -1;
}

// Suspend a group of processs.

int
ACE_Process_Manager::apply_grp (int grp_id, 
			       PROC_FUNC func,
			       int arg)
{
  ACE_TRACE ("ACE_Process_Manager::apply_grp");
  return -1;
}

int 
ACE_Process_Manager::suspend_grp (int grp_id)
{
  ACE_TRACE ("ACE_Process_Manager::suspend_grp");
  return -1;
}

// Resume a group of processs.

int 
ACE_Process_Manager::resume_grp (int grp_id)
{
  ACE_TRACE ("ACE_Process_Manager::resume_grp");
  return -1;
}

// Kill a group of processs.

int 
ACE_Process_Manager::kill_grp (int grp_id, int signum)
{
  ACE_TRACE ("ACE_Process_Manager::kill_grp");
  return -1;
}

int
ACE_Process_Manager::apply_all (PROC_FUNC func, int arg)
{
  ACE_TRACE ("ACE_Process_Manager::apply_all");
  return -1;
}

// Resume all processs that are suspended.

int 
ACE_Process_Manager::resume_all (void)
{
  ACE_TRACE ("ACE_Process_Manager::resume_all");
  return -1;
}

int 
ACE_Process_Manager::suspend_all (void)
{
  ACE_TRACE ("ACE_Process_Manager::suspend_all");
  return -1;
}

int 
ACE_Process_Manager::kill_all (int sig)
{
  ACE_TRACE ("ACE_Process_Manager::kill_all");
  return -1;
}

// Must be called when process goes out of scope to clean up its table
// slot.

void *
ACE_Process_Manager::exit (void *status)
{
  ACE_TRACE ("ACE_Process_Manager::exit");
  return 0;
}

// Wait for all the processs to exit.

int
ACE_Process_Manager::wait (ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Process_Manager::wait");
  return -1;
}

void
ACE_Process_Control::dump (void) const
{
  ACE_TRACE ("ACE_Process_Control::dump");
}

// Initialize the process controller.

ACE_Process_Control::ACE_Process_Control (ACE_Process_Manager *t, 
					  int insert)
{
  ACE_TRACE ("ACE_Process_Control::ACE_Process_Control");
}

// Automatically kill process on exit.

ACE_Process_Control::~ACE_Process_Control (void)
{
  ACE_TRACE ("ACE_Process_Control::~ACE_Process_Control");
}

// Exit from process (but clean up first).

void *
ACE_Process_Control::exit (void *exit_status)
{
  ACE_TRACE ("ACE_Process_Control::exit");
  return 0;
}

