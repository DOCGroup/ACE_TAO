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

ACE_RCSID (ace, Process_Manager, "$Id$")

ACE_ALLOC_HOOK_DEFINE (ACE_Process_Manager)

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

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("\nproc_id_ = %d"),
              this->process_->getpid()));

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

ACE_Process_Descriptor::~ACE_Process_Descriptor (void)
{
}

ACE_Process_Descriptor::ACE_Process_Descriptor (void)
  : delete_process_ (0),
	process_ (0),
	exit_notify_ (0)
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
       i < this->current_count_;
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
ACE_Process_Manager::open (size_t size,
                           ACE_Reactor *r)
{
  ACE_TRACE ("ACE_Process_Manager::open");

  if (r) 
    {
      ACE_Event_Handler::reactor (r);
#if !defined (ACE_WIN32)
      // (No signals for child-exited on Win32) Assign the
      // Process_Manager a dummy I/O descriptor.  Note that even
      // though we open this file "Write Only" we still need to use
      // the ACE_Event_Handler::NULL_MASK when registering this with
      // the ACE_Reactor (see below).
      this->dummy_handle_ = ACE_OS::open (ACE_DEV_NULL,
                                          O_WRONLY);
      ACE_ASSERT (this->dummy_handle_ != ACE_INVALID_HANDLE);
  
      // Register signal handler object.  Note that NULL_MASK is used
      // to keep the ACE_Reactor from calling us back on the
      // "/dev/null" descriptor.  NULL_MASK just reserves a "slot" in
      // the Reactor's internal demuxing table, but doesn't cause it
      // to dispatch the event handler directly.  Instead, we use the
      // signal handler to do this.
      if (reactor ()->register_handler
          (this,
           ACE_Event_Handler::NULL_MASK) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n%a",
                    "register_handler",
                    1));

      if (reactor ()->register_handler
          (SIGCHLD,
           this) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n%a",
                    "register_handler",
                    1));
#endif	// !defined (ACE_WIN32)
    }
  
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->max_process_table_size_ < size)
    this->resize (size);
  return 0;

}

// Initialize the synchronization variables.

ACE_Process_Manager::ACE_Process_Manager (size_t size,ACE_Reactor *r)
  : ACE_Event_Handler(),
	process_table_ (0),
    max_process_table_size_ (0), 
    current_count_ (0),
	dummy_handle_ (ACE_INVALID_HANDLE),
	default_exit_handler_ (0)
#if defined (ACE_HAS_THREADS)
    , zero_cond_ (lock_)
#endif /* ACE_HAS_THREADS */
{
  ACE_TRACE ("ACE_Process_Manager::ACE_Process_Manager");

  if (this->open (size,r) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Process_Manager")));
}

// Close up and release all resources.

int
ACE_Process_Manager::close (void)
{
  ACE_TRACE ("ACE_Process_Manager::close");

  if (this->reactor ())
    {
      this->reactor ()->remove_handler (this, 0);
      this->reactor (0);
    }

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->process_table_ != 0)
    {
      for (size_t i = 0; i < this->current_count_; ++i) 
        if (this->process_table_[i].exit_notify_ != 0)
          this->process_table_[i].exit_notify_->handle_close
            (this->process_table_[i].process_->gethandle (),
             0);

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

ACE_HANDLE
ACE_Process_Manager::get_handle (void) const
{
  return this->dummy_handle_;
}

// This is called when the Reactor notices that a process has exited.
// On Windoze, it knows which process it was, and passes the process'
// handle.  On Unix, what has actually happened is a SIGCHLD invoked
// the handle_signal routine, which fooled the Reactor into thinking
// that this routine needed to be called. (On Unix, we reap as many
// children as are dead.)

int
ACE_Process_Manager::handle_input (ACE_HANDLE proc)
{
  ACE_TRACE ("ACE_Process_Manager::handle_input");

#if defined (ACE_WIN32)
  DWORD status = 0;
  BOOL result = ::GetExitCodeProcess (proc, &status);

  if (result) 
    {
      if (status != STILL_ACTIVE) 
        {
          {
            ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex,ace_mon,lock_,-1));
            int	 i = find_proc ( proc );
            pid_t pid = i != -1 ? process_table_[i].process_->getpid () : -1;
            this->notify_proc_handler (proc, pid, status);
            this->remove_proc (pid);
          }
          ACE_Reactor *r = reactor();
          if (r)
            r->remove_handler ( proc, 0 );
        } 
      else
        {
          // Huh?  Process still active -- shouldn't have been called yet!
        }
    } 
  else 
    {
      // <GetExitCodeProcess> failed.
      ACE_ERROR ((LM_ERROR,
                  "%p\n%a",
                  "handle_input: GetExitCodeProcess failed",
                  0));
    }
#else	// !defined (ACE_WIN32)
  ACE_UNUSED_ARG (proc);	
  // <proc> is <dummy_handle_> on unix.
  while (this->reap() > 0) 
    continue;
#endif /* ACE_WIN32 */
  return 0;
}

int
ACE_Process_Manager::handle_signal (int, siginfo_t *, ucontext_t *)
{
  return reactor ()->ready_ops
    (this->dummy_handle_,
     ACE_Event_Handler::READ_MASK,
     ACE_Reactor::ADD_MASK);
}

int
ACE_Process_Manager::register_handler (ACE_Event_Handler *eh, pid_t pid)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (pid == -1) 
    {
      if (this->default_exit_handler_ != 0)
        this->default_exit_handler_->handle_close (ACE_INVALID_HANDLE,
                                                   0);
      this->default_exit_handler_ = eh;
      return 0;
    }

  int i = this->find_proc (pid);

  if (i == -1)
    return -1;
  else
    {
      ACE_Process_Descriptor &proc_desc = this->process_table_[i];

      if (proc_desc.exit_notify_ != 0)
        proc_desc.exit_notify_->handle_close (ACE_INVALID_HANDLE, 0);
      proc_desc.exit_notify_ = eh;
      return 0;
    }
}

int
ACE_Process_Manager::handle_close (ACE_HANDLE handle,
                                   ACE_Reactor_Mask)
{
  ACE_TRACE ("ACE_Process_Manager::handle_close");

  ACE_ASSERT (handle == this->dummy_handle_);

  ACE_OS::close (dummy_handle_);

  return 0;
}
	
// Create a new process.

pid_t
ACE_Process_Manager::spawn (ACE_Process_Options &options)
{
  ACE_Process *process;
  ACE_NEW_RETURN (process,
                  ACE_Process,
                  ACE_INVALID_PID);

  pid_t pid = spawn (process,
                     options);

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  int i = ACE_INVALID_PID;

  if (pid != -1)
    i = this->find_proc (pid);

  if (i != -1)
    this->process_table_[i].delete_process_ = 1;

  return pid;
}

// Create a new process.

pid_t
ACE_Process_Manager::spawn (ACE_Process *process,
                            ACE_Process_Options &options)
{
  ACE_TRACE ("ACE_Process_Manager::spawn");

  pid_t pid = process->spawn (options);

  // Only include the pid in the parent's table.
  if (pid == -1 || pid == 0)
    return pid;
  else
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

      if (this->append_proc (process) == -1)
        return ACE_INVALID_PID;
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
ACE_Process_Manager::append_proc (ACE_Process *proc)
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

      proc_desc.delete_process_ = 0;	// pending better info from caller
      proc_desc.process_ = proc;

#if defined (ACE_WIN32)
      // If we have a Reactor, then we're supposed to reap Processes
      // automagically.  Get a handle to this new Process and tell the
      // Reactor we're interested in handling_input () on it.

      ACE_Reactor *r = this->reactor ();
      if (r != 0)
        r->register_handler (proc->gethandle (),
                             this,
                             ACE_Event_Handler::READ_MASK);
#endif /* ACE_WIN32 */

      this->current_count_++;
      return 0;
    }
}

// Insert a process into the pool (checks for duplicates and doesn't
// allow them to be inserted twice).

int
ACE_Process_Manager::insert_proc (ACE_Process *proc)
{
  ACE_TRACE ("ACE_Process_Manager::insert_proc");

#if 0
  // Check for duplicates and bail out if they're already
  // registered...
  if (this->find_proc (proc->getpid ()) != -1)
    return -1;

  return this->append_proc (proc);
#else
  ACE_UNUSED_ARG (proc);
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
      if (this->process_table_[i].delete_process_)
        delete this->process_table_[i].process_;
	  
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
      int result = ACE::terminate_process (this->process_table_[i].process_->getpid ());

      if (result != -1)
	{ 
          // Save/restore errno.
          ACE_Errno_Guard error (errno);
	  this->remove (this->process_table_[i].process_->getpid ()); 
	  return 0; 
	} 
      else 
	return -1; 
    }
}

int
ACE_Process_Manager::terminate (pid_t pid, int sig)
{
  ACE_TRACE ("ACE_Process_Manager::terminate");

  // Check for duplicates and bail out if they're already
  // registered...
  int i = this->find_proc (pid);

  if (i == -1)
    return -1;
  else
    {
		return ACE_OS::kill (this->process_table_[i].process_->getpid (), sig);
    }
}

// Locate the index in the table associated with <pid>.  Must be
// called with the lock held.

int 
ACE_Process_Manager::find_proc (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::find_proc");

  for (size_t i = 0; i < this->current_count_; i++)
    if (pid == this->process_table_[i].process_->getpid ())
      return i;

  return -1;    
}

#if defined (ACE_WIN32)
// Locate the index in the table associated with <h>.  Must be called
// with the lock held.

int 
ACE_Process_Manager::find_proc (ACE_HANDLE h)
{
  ACE_TRACE ("ACE_Process_Manager::find_proc");

  for (size_t i = 0; i < this->current_count_; i++)
    if (h == this->process_table_[i].process_->gethandle ())
      return i;

  return -1;    
}
#endif /* ACE_WIN32 */

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

      pid_t pid = proc_desc.process_->wait ();
      
      if (pid != -1)
        this->remove_proc (pid);
      return pid;
    }

  return 0;
}

// Collect a single child processes' exit status by calling
// <ACE_OS::wait>.  Calls the appropriate exit_notify, if registered.

int 
ACE_Process_Manager::wait (pid_t pid,
                           int *stat_loc,
                           int options)
{
  ACE_TRACE ("ACE_Process_Manager::wait");

  int local_stat = 0;

  if (stat_loc == 0)
    stat_loc = &local_stat;

  pid = ACE_OS::waitpid (pid, stat_loc, options);

  if (pid > 0) 
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
      this->notify_proc_handler (ACE_HANDLE (pid),
                                 pid,
                                 *stat_loc);
    }

  return pid;
}

int 
ACE_Process_Manager::reap (pid_t pid,
                           int *stat_loc,
                           int options)
{
  ACE_TRACE ("ACE_Process_Manager::reap");

  return this->wait (pid, stat_loc, options);
}

// Notify either the process-specific handler or the generic handler.
// If process-specific, call handle_close on the handler.  Returns 1
// if process found, 0 if not.  Must be called with locks held.

int
ACE_Process_Manager::notify_proc_handler (ACE_HANDLE h,
                                          pid_t pid,
                                          int)
{
  int i = this->find_proc (pid);

  if (i != -1) 
    {
      ACE_Process_Descriptor &proc_desc = this->process_table_[i];

      if (proc_desc.exit_notify_ != 0) 
        {
          proc_desc.exit_notify_->handle_input (h);
          proc_desc.exit_notify_->handle_close (h, 0);
          proc_desc.exit_notify_ = 0;
        }
      else if (this->default_exit_handler_ != 0
               && this->default_exit_handler_->handle_input (h) < 0)
        this->register_handler (0);
    } 
  else
    ACE_DEBUG ((LM_DEBUG,
                "(%P:%t|%T) ACE_Process_Manager::notify_proc_handler: unknown process %d reaped\n",
                pid));

  return i != -1;
}
