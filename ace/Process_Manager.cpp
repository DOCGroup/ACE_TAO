// $Id$

// Process_Manager.cpp
#define ACE_BUILD_DLL
#include "ace/Synch_T.h"
#include "ace/Process.h"
#include "ace/Process_Manager.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Process_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Process_Manager, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Process_Manager)

// Singleton instance.
ACE_Process_Manager *ACE_Process_Manager::instance_ = 0;

// Controls whether the <Process_Manager> is deleted when we shut down
// (we can only delete it safely if we created it!)
int ACE_Process_Manager::delete_instance_ = 0;

void
ACE_Process_Descriptor::dump (void) const
{
  ACE_TRACE ("ACE_Process_Descriptor::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nproc_id_ = %d"), this->proc_id_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\ngrp_id_ = %d"), this->grp_id_));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_Process_Manager::dump (void) const
{
  ACE_TRACE ("ACE_Process_Manager::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nmax_process_table_size_ = %d"), this->max_process_table_size_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\ncurrent_count_ = %d"), this->current_count_));

  for (size_t i = 0; i < this->current_count_; i++)
    this->process_table_[i].dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Process_Descriptor::ACE_Process_Descriptor (void)
{
  ACE_TRACE ("ACE_Process_Descriptor::ACE_Process_Descriptor");
}

ACE_Process_Manager *
ACE_Process_Manager::instance (void)
{
  ACE_TRACE ("ACE_Process_Manager::instance");

  if (ACE_Process_Manager::instance_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));

      if (ACE_Process_Manager::instance_ == 0)
        {
          ACE_NEW_RETURN (ACE_Process_Manager::instance_,
                          ACE_Process_Manager,
                          0);
          ACE_Process_Manager::delete_instance_ = 1;
        }
    }

  return ACE_Process_Manager::instance_;
}

ACE_Process_Manager *
ACE_Process_Manager::instance (ACE_Process_Manager *tm)
{
  ACE_TRACE ("ACE_Process_Manager::instance");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                            *ACE_Static_Object_Lock::instance (), 0));

  ACE_Process_Manager *t = ACE_Process_Manager::instance_;
  // We can't safely delete it since we don't know who created it!
  ACE_Process_Manager::delete_instance_ = 0;

  ACE_Process_Manager::instance_ = tm;
  return t;
}

int
ACE_Process_Manager::resize (size_t size)
{
  ACE_TRACE ("ACE_Process_Manager::resize");

  ACE_Process_Descriptor *temp;
  
  ACE_NEW_RETURN (temp,
                  ACE_Process_Descriptor[size],
                  -1);

  for (size_t i = 0;
       i < this->max_process_table_size_;
       i++)
    // Structure assignment.
    temp[i] = this->process_table_[i]; 

  this->max_process_table_size_ = size;

  delete [] this->process_table_;

  this->process_table_ = temp;
  return 0;
}

// Create and initialize the table to keep track of the process pool.

int
ACE_Process_Manager::open (size_t size)
{
  ACE_TRACE ("ACE_Process_Manager::open");

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->max_process_table_size_ < size)
    this->resize (size);
  return 0;

}

// Initialize the synchronization variables.

ACE_Process_Manager::ACE_Process_Manager (size_t size)
  : process_table_ (0),
    max_process_table_size_ (0), 
    current_count_ (0)
#if defined (ACE_HAS_THREADS)
    , zero_cond_ (lock_)
#endif /* ACE_HAS_THREADS */
{
  ACE_TRACE ("ACE_Process_Manager::ACE_Process_Manager");

  if (this->open (size) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Process_Manager")));
}

// Close up and release all resources.

int
ACE_Process_Manager::close (void)
{
  ACE_TRACE ("ACE_Process_Manager::close");

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->process_table_ != 0)
    {
      delete [] this->process_table_;
      this->process_table_ = 0;
      this->max_process_table_size_ = 0;
      this->current_count_ = 0;
    }
  return 0;
}

ACE_Process_Manager::~ACE_Process_Manager (void)
{
  ACE_TRACE ("ACE_Process_Manager::~ACE_Process_Manager");
  this->close ();
}

// Create a new process.  *Must* be called with the lock_ held...

pid_t
ACE_Process_Manager::spawn (ACE_Process_Options &options)
{
  ACE_TRACE ("ACE_Process_Manager::spawn");

  ACE_Process process;
  pid_t pid = process.spawn (options);

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

// Create N new processs.

int 
ACE_Process_Manager::spawn_n (size_t n,
                              ACE_Process_Options &options,
                              pid_t *child_pids)
{
  ACE_TRACE ("ACE_Process_Manager::spawn_n");

  // This doesn't work (yet).
  for (size_t i = 0; i < n; i++)
    {
      pid_t pid = this->spawn (options);
      if (pid <= 0)
        // We're in the child or something's gone wrong.
        return pid;
      else if (child_pids != 0)
        child_pids[i] = pid;
    }

  return 0;
}

// Append a process into the pool (does not check for duplicates).
// Must be called with locks held.

int
ACE_Process_Manager::append_proc (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::append_proc");

  // Try to resize the array to twice its existing size if we run out
  // of space...
  if (this->current_count_ >= this->max_process_table_size_ 
      && this->resize (this->max_process_table_size_ * 2) == -1)
    return -1;
  else
    {
      ACE_Process_Descriptor &proc_desc = 
	this->process_table_[this->current_count_];

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

// Remove a process from the pool.

int
ACE_Process_Manager::remove (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::remove");

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  return this->remove_proc (pid);
}

// Remove a process from the pool.  Must be called with locks held.

int
ACE_Process_Manager::remove_proc (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::remove");

  int i = this->find_proc (pid);

  if (i == -1)
    return -1;
  else
    {
      this->current_count_--;

      if (this->current_count_ > 0)
	// Compact the table by moving the last item into the slot
	// vacated by the index being removed (this is a structure
	// assignment).
	this->process_table_[i] =
          this->process_table_[this->current_count_];

#if defined (ACE_HAS_THREADS)
      // Tell all waiters when there are no more threads left in the
      // pool.
      if (this->current_count_ == 0)
	this->zero_cond_.broadcast ();
#endif /* ACE_HAS_THREADS */
      return 0;
    }
}

int
ACE_Process_Manager::terminate (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::terminate");

  // Check for duplicates and bail out if they're already
  // registered...
  int i = this->find_proc (pid);

  if (i == -1)
    return -1;
  else
    {
      int result = ACE::terminate_process (this->process_table_[i].proc_id_);

      if (result != -1)
	{ 
          // Save/restore errno.
          ACE_Errno_Guard error (errno);
	  this->remove (this->process_table_[i].proc_id_); 
	  return 0; 
	} 
      else 
	return -1; 
    }
}

// Locate the index in the table associated with <pid>.  Must be
// called with the lock held.

int 
ACE_Process_Manager::find_proc (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::find_proc");

  for (size_t i = 0; i < this->current_count_; i++)
    if (pid == this->process_table_[i].proc_id_)
      return i;

  return -1;    
}

// Wait for all the processs to exit.  This implementation requires a
// cooperative signal handler or <ACE_OS::sigwait> thread, so it's not
// portable to Win32.

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

// The following method *is* portable all the operating systems that
// support processes.

int
ACE_Process_Manager::wait (void)
{
  ACE_TRACE ("ACE_Process_Manager::wait");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  for (size_t i = 0; i < this->current_count_; i++)
    {
      ACE_Process_Descriptor &proc_desc = 
	this->process_table_[i];

      pid_t pid = ACE_OS::wait (proc_desc.proc_id_, 0);
      
      if (pid == -1)
        return -1;
      else
        this->remove_proc (pid);
    }

  return 0;
}

// Reap a single child processes' exit status by calling
// <ACE_OS::wait>.

int 
ACE_Process_Manager::reap (pid_t pid,
                           int *stat_loc,
                           int options)
{
  ACE_TRACE ("ACE_Process_Manager::reap");

  pid = ACE_OS::wait (pid, stat_loc, options);

  if (pid != -1)
    this->remove (pid);
  return pid;
}

int 
ACE_Process_Manager::wait (pid_t pid,
                           int *stat_loc,
                           int options)
{
  ACE_TRACE ("ACE_Process_Manager::wait");

  return this->reap (pid, stat_loc, options);
}
