// $Id$

// Process_Manager.cpp
#define ACE_BUILD_DLL
#include "ace/Process.h"
#include "ace/Process_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Process_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Process_Manager)

void
ACE_Process_Descriptor::dump (void) const
{
  ACE_TRACE ("ACE_Process_Descriptor::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, "\nproc_id_ = %d", this->proc_id_));
  ACE_DEBUG ((LM_DEBUG, "\ngrp_id_ = %d", this->grp_id_));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_Process_Manager::dump (void) const
{
  ACE_TRACE ("ACE_Process_Manager::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, "\nmax_table_size_ = %d", this->max_table_size_));
  ACE_DEBUG ((LM_DEBUG, "\ncurrent_count_ = %d", this->current_count_));

  for (size_t i = 0; i < this->current_count_; i++)
    this->proc_table_[i].dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Process_Descriptor::ACE_Process_Descriptor (void)
{
  ACE_TRACE ("ACE_Process_Descriptor::ACE_Process_Descriptor");
}

int
ACE_Process_Manager::resize (size_t size)
{
  ACE_TRACE ("ACE_Process_Manager::resize");

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
ACE_Process_Manager::start (ACE_Process_Options &options)
{
  ACE_TRACE ("ACE_Process_Manager::start");

  ACE_Process process;
  pid_t pid = process.start (options);

  // Only include the pid in the parent's table.
  if (pid == -1 || pid == 0)
    return pid;
  else
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

      if (this->append_proc (pid) == -1)
	return -1;
      else
	return pid;
    }
}

// Create N new processs running FUNC.

int 
ACE_Process_Manager::start_n (size_t n, ACE_Process_Options &options)
{
  ACE_TRACE ("ACE_Process_Manager::spawn_n");

#if 0
  // This doesn't work (yet).
  for (size_t i = 0; i < n; i++)
    if (this->start (options) == -1)
      return -1;

  return 0;
#else
  ACE_UNUSED_ARG (n);
  ACE_UNUSED_ARG (options);
  ACE_NOTSUP_RETURN (-1);
#endif /* 0 */
}

// Append a process into the pool (does not check for duplicates).
// Must be called with locks held.

int
ACE_Process_Manager::append_proc (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::append_proc");

  // Try to resize the array to twice its existing size if we run out
  // of space...
  if (this->current_count_ >= this->max_table_size_ 
      && this->resize (this->max_table_size_ * 2) == -1)
    return -1;
  else
    {
      ACE_Process_Descriptor &proc_desc = 
	this->proc_table_[this->current_count_];

      proc_desc.proc_id_ = pid;
      proc_desc.grp_id_ = ACE_OS::getpgid (pid);

      this->current_count_++;
      return 0;
    }
}

// Insert a process into the pool (checks for duplicates and doesn't
// allow them to be inserted twice).

int
ACE_Process_Manager::insert_proc (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::insert_proc");

#if 0
  // Check for duplicates and bail out if they're already
  // registered...
  if (this->find_proc (pid) != -1)
    return -1;

  return this->append_proc (pid);
#else
  ACE_UNUSED_ARG (pid);
  ACE_NOTSUP_RETURN (-1);
#endif 
}

// Remove a process from the pool.  Must be called with locks held.

int
ACE_Process_Manager::remove (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::remove");

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  int i = this->find_proc (pid);

  if (i == -1)
    return -1;
  else
    {
      this->current_count_--;

      if (this->current_count_ > 0)
	// Compact the table by moving the last item into the slot vacated
	// by the index being removed (this is a structure assignment).
	this->proc_table_[i] = this->proc_table_[this->current_count_];

#if defined (ACE_HAS_THREADS)
      // Tell all waiters when there are no more threads left in the pool.
      if (this->current_count_ == 0)
	this->zero_cond_.broadcast ();
#endif /* ACE_HAS_THREADS */
      return 0;
    }
}

int
ACE_Process_Manager::kill (pid_t pid, int signum)
{
  ACE_TRACE ("ACE_Process_Manager::kill_proc");

  // Check for duplicates and bail out if they're already
  // registered...
  int i = this->find_proc (pid);

  if (i == -1)
    return -1;
  else
    {
      int result = ACE_OS::kill (this->proc_table_[i].proc_id_,
				 signum);

      if (result == -1)
	{ 
	  // We need to save this across calls to remove_thr() since that
	  // call may reset errno.
	  int error = errno;

	  this->remove (this->proc_table_[i].proc_id_); 
	  errno = error; 
	  return -1; 
	} 
      else 
	return 0; 
    }
}

// Locate the index in the table associated with <pid>.  Must be
// called with the lock held.

int 
ACE_Process_Manager::find_proc (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::find_proc");

  for (size_t i = 0; i < this->current_count_; i++)
    if (pid == this->proc_table_[i].proc_id_)
      return i;

  return -1;    
}

// Wait for all the processs to exit.

int
ACE_Process_Manager::wait (ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Process_Manager::wait");

#if defined (ACE_HAS_THREADS)
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

  while (this->current_count_ > 0)
    if (this->zero_cond_.wait (timeout) == -1)
      return -1;
#else
  ACE_UNUSED_ARG (timeout);
#endif /* ACE_HAS_THREADS */

  return 0;
}

// Reap a <SIGCHLD> by calling <ACE_OS::waitpid>.

int 
ACE_Process_Manager::reap (pid_t pid, int *stat_loc, int options)
{
  ACE_TRACE ("ACE_Process_Manager::reap");

  pid = ACE_OS::waitpid (pid, stat_loc, options);

  if (pid != -1)
    this->remove (pid);
  return pid;
}
