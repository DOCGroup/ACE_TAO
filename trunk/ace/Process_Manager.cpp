// $Id$

// Process_Manager.cpp
#include "ace/Synch_T.h"
#include "ace/Process.h"
#include "ace/Signal.h"
#include "ace/Process_Manager.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Process_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Process_Manager, "$Id$")

class ACE_Managed_Process : public ACE_Process
{
  // = TITLE
  //   <ACE_Managed_Process> is just an <ACE_Process> with an
  //   <unmanage> method that deletes the instance.
public:
  ACE_Managed_Process (void);
  // Constructor.

  virtual void unmanage (void);
  // Cleanup by deleting <this>.

private:
  virtual ~ACE_Managed_Process (void);
  // Make sure that we're allocated dynamically!

  friend class ace_dewarn_gplusplus;
  // Keep G++ happy...
};

ACE_Managed_Process::ACE_Managed_Process (void)
{
}

ACE_Managed_Process::~ACE_Managed_Process (void)
{
}

void
ACE_Managed_Process::unmanage (void)
{
  delete this;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Process_Manager)

// Singleton instance.
ACE_Process_Manager *ACE_Process_Manager::instance_ = 0;

// Controls whether the <Process_Manager> is deleted when we shut down
// (we can only delete it safely if we created it!)
int ACE_Process_Manager::delete_instance_ = 0;

ACE_Process_Descriptor::~ACE_Process_Descriptor (void)
{
}

void
ACE_Process_Descriptor::dump (void) const
{
  ACE_TRACE ("ACE_Process_Descriptor::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG,  ACE_TEXT ("\nproc_id_ = %d"),
                          this->process_->getpid( )));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_Process_Manager::dump (void) const
{
  ACE_TRACE ("ACE_Process_Manager::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG,  ACE_TEXT ("\nmax_process_table_size_ = %d"), this->max_process_table_size_));
  ACE_DEBUG ((LM_DEBUG,  ACE_TEXT ("\ncurrent_count_ = %d"), this->current_count_));

  for (size_t i = 0; i < this->current_count_; i++)
    this->process_table_[i].dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Process_Descriptor::ACE_Process_Descriptor (void)
  : process_ (0),
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

#if !defined (ACE_LACKS_SETPGID)
  // Set up a process group so that the thread that opened this
  // Manager will be able to put children into its own group and wait
  // for them.
   if (ACE_OS::setpgid (0, 0) == -1)
     ACE_ERROR ((LM_WARNING,
                 ACE_TEXT ("%p.\n"),
                 ACE_TEXT ("ACE_Process_Manager::open: can't create a ")
                 ACE_TEXT ("process group; some wait functions may fail")));
#endif /* ACE_LACKS_SETPGID */

  if (r)
    {
      ACE_Event_Handler::reactor (r);
#if !defined(ACE_WIN32) && !defined (ACE_PSOS)
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
          (SIGCHLD, this) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n%a",
                    "register_handler",
                    1));
#endif  // !defined(ACE_WIN32) && !defined (ACE_PSOS)
    }

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->max_process_table_size_ < size)
    this->resize (size);
  return 0;
}

// Initialize the synchronization variables.

ACE_Process_Manager::ACE_Process_Manager (size_t size,
                                          ACE_Reactor *r)
  : ACE_Event_Handler (),
    process_table_ (0),
    max_process_table_size_ (0),
    current_count_ (0),
#if !defined(ACE_WIN32)
    dummy_handle_ (ACE_INVALID_HANDLE),
#endif // !defined(ACE_WIN32)
    default_exit_handler_ (0)
#if defined (ACE_HAS_THREADS)
  , lock_ ()
#endif /* ACE_HAS_THREADS */
{
  ACE_TRACE ("ACE_Process_Manager::ACE_Process_Manager");

  if (this->open (size,
                  r) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_Process_Manager")));
}

// Close up and release all resources.

int
ACE_Process_Manager::close (void)
{
  ACE_TRACE ("ACE_Process_Manager::close");

#if !defined (ACE_WIN32)
  if (this->reactor ())
    {
      this->reactor ()->remove_handler (this, 0);
      this->reactor (0);
    }
#endif /*  !ACE_WIN32  */

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->process_table_ != 0)
    {
      while (this->current_count_ > 0)
        this->remove_proc (0);

      delete [] this->process_table_;
      this->process_table_ = 0;
      this->max_process_table_size_ = 0;
      this->current_count_ = 0;
    }

  if (this->default_exit_handler_ != 0)
      this->default_exit_handler_->handle_close (ACE_INVALID_HANDLE,0);
  this->default_exit_handler_ = 0;

  return 0;
}

ACE_Process_Manager::~ACE_Process_Manager (void)
{
  ACE_TRACE ("ACE_Process_Manager::~ACE_Process_Manager");
  this->close ();
}

#if !defined (ACE_WIN32)

// This is called when the Reactor notices that a Process has exited.
// What has actually happened is a SIGCHLD invoked the <handle_signal>
// routine, which fooled the Reactor into thinking that this routine
// needed to be called.  Since we don't know which Process exited, we
// must reap as many exit statuses as are immediately available.

int
ACE_Process_Manager::handle_input (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Process_Manager::handle_input");

   pid_t pid;

   do
     pid = this->wait (0,
                       ACE_Time_Value::zero);
   while (pid != 0 && pid != ACE_INVALID_PID);

  return 0;
}

ACE_HANDLE
ACE_Process_Manager::get_handle (void) const
{
  return this->dummy_handle_;
}
#endif /* !ACE_WIN32 */

// On Unix, this routine is called asynchronously when a SIGCHLD is
// received.  We just tweak the reactor so that it'll call back our
// <handle_input> function, which allows us to handle Process exits
// synchronously.
//
// On Win32, this routine is called synchronously, and is passed the
// HANDLE of the Process that exited, so we can do all our work here.

int
ACE_Process_Manager::handle_signal (int,
                                    siginfo_t *si,
                                    ucontext_t *)
{
#if defined (ACE_WIN32)
  ACE_HANDLE proc = si->si_handle_;
  ACE_exitcode status = 0;
  BOOL result = ::GetExitCodeProcess (proc,
                                      &status);
  if (result)
    {
      if (status != STILL_ACTIVE)
        {
          {
            ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, lock_, -1));

            ssize_t i = this->find_proc (proc);
            pid_t pid = i != -1
              ? process_table_[i].process_->getpid ()
              : ACE_INVALID_PID;
            this->notify_proc_handler (i, status);
            this->remove_proc (i);
          }
          return -1; // remove this HANDLE/Event_Handler combination
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Process still active")
                           ACE_TEXT (" -- shouldn't have been called yet!\n")),
                          0); // return 0 : stay registered
    }
  else
    {
      // <GetExitCodeProcess> failed.
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("GetExitCodeProcess failed")),
                        -1); // return -1: unregister
    }
#else /* !ACE_WIN32 */
  ACE_UNUSED_ARG (si);
  return reactor ()->ready_ops
    (this->dummy_handle_,
     ACE_Event_Handler::READ_MASK,
     ACE_Reactor::ADD_MASK);
#endif /* !ACE_WIN32 */
}

int
ACE_Process_Manager::register_handler (ACE_Event_Handler *eh,
                                       pid_t pid)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (pid == ACE_INVALID_PID)
    {
      if (this->default_exit_handler_ != 0)
        this->default_exit_handler_->handle_close
          (ACE_INVALID_HANDLE,
           0);
      this->default_exit_handler_ = eh;
      return 0;
    }

  ssize_t i = this->find_proc (pid);

  if (i == -1)
    // set "process not found" error
    return -1;
  else
    {
      ACE_Process_Descriptor &proc_desc = this->process_table_[i];

      if (proc_desc.exit_notify_ != 0)
        proc_desc.exit_notify_->handle_close
          (ACE_INVALID_HANDLE,
           0);
      proc_desc.exit_notify_ = eh;
      return 0;
    }
}

int
ACE_Process_Manager::handle_close (ACE_HANDLE handle,
                                   ACE_Reactor_Mask)
{
  ACE_TRACE ("ACE_Process_Manager::handle_close");
  ACE_UNUSED_ARG (handle);

#if !defined (ACE_WIN32)
  ACE_ASSERT (handle == this->dummy_handle_);

  ACE_OS::close (dummy_handle_);
#endif /* ACE_WIN32 */
  return 0;
}

// Create a new process.

pid_t
ACE_Process_Manager::spawn (ACE_Process_Options &options)
{
  ACE_Process *process;
  ACE_NEW_RETURN (process,
                  ACE_Managed_Process,
                  ACE_INVALID_PID);

  return spawn (process, options);
}

// Create a new process.

pid_t
ACE_Process_Manager::spawn (ACE_Process *process,
                            ACE_Process_Options &options)
{
  ACE_TRACE ("ACE_Process_Manager::spawn");

  if (options.getgroup () == ACE_INVALID_PID)
    options.setgroup (ACE_OS::getpid ());

  pid_t pid = process->spawn (options);

  // Only include the pid in the parent's table.
  if (pid == ACE_INVALID_PID
      || pid == 0)
    return pid;
  else
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex,
                                ace_mon, this->lock_, -1));

      if (this->append_proc (process) == -1)
        // bad news: spawned, but not registered in table.
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

  if (child_pids != 0)
    for (size_t i = 0;
         i < n;
         ++i)
      child_pids[i] = ACE_INVALID_PID;

  for (size_t i = 0;
       i < n;
       i++)
    {
      pid_t pid = this->spawn (options);
      if (pid == ACE_INVALID_PID || pid == 0)
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

      proc_desc.process_ = proc;
      proc_desc.exit_notify_ = 0;

#if defined (ACE_WIN32)
      // If we have a Reactor, then we're supposed to reap Processes
      // automagically.  Get a handle to this new Process and tell the
      // Reactor we're interested in <handling_input> on it.

      ACE_Reactor *r = this->reactor ();
      if (r != 0)
        r->register_handler (this,
                             proc->gethandle ());
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

  // Check for duplicates and bail out if they're already
  // registered...
  if (this->find_proc (proc->getpid ()) != -1)
    return -1;

  return this->append_proc (proc);
}

// Remove a process from the pool.

int
ACE_Process_Manager::remove (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::remove");

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ssize_t i = this->find_proc (pid);

  if (i != -1)
    return this->remove_proc (i);
  else
    // set "process not found" error
    return -1;
}

// Remove a process from the pool.  Must be called with locks held.

int
ACE_Process_Manager::remove_proc (size_t i)
{
  ACE_TRACE ("ACE_Process_Manager::remove");

  // If there's an exit_notify_ <Event_Handler> for this pid, call its
  // <handle_close> method.

  if (this->process_table_[i].exit_notify_ != 0)
    {
      this->process_table_[i].exit_notify_->handle_close
        (this->process_table_[i].process_->gethandle(),
         0);
      this->process_table_[i].exit_notify_ = 0;
    }

#if defined (ACE_WIN32)
  ACE_Reactor *r = this->reactor ();
  if (r != 0)
    r->remove_handler (this->process_table_[i].process_->gethandle (),
                       ACE_Event_Handler::DONT_CALL);
#endif /* ACE_WIN32 */

  this->process_table_[i].process_->unmanage ();

  this->process_table_[i].process_ = 0;

  this->current_count_--;

  if (this->current_count_ > 0)
    // Compact the table by moving the last item into the slot vacated
    // by the index being removed (this is a structure assignment).
    this->process_table_[i] =
      this->process_table_[this->current_count_];

  return 0;
}

int
ACE_Process_Manager::terminate (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::terminate");

  // Check for duplicates and bail out if they're already
  // registered...
  ssize_t i = this->find_proc (pid);

  if (i == -1)
    // set "no such process" error
    return -1;

  int result = ACE::terminate_process (pid);

  if (result != -1)
    {
      // Save/restore errno.
      ACE_Errno_Guard error (errno);
      this->remove (i);
      return 0;
    }
  else
    return -1;
}

int
ACE_Process_Manager::terminate (pid_t pid,
                                int sig)
{
  ACE_TRACE ("ACE_Process_Manager::terminate");

  // Check for duplicates and bail out if they're already
  // registered...
  ssize_t i = this->find_proc (pid);

  if (i == -1)
    // set "no such process" error
    return -1;

  return ACE_OS::kill (pid, sig);
}

// Locate the index in the table associated with <pid>.  Must be
// called with the lock held.

ssize_t
ACE_Process_Manager::find_proc (pid_t pid)
{
  ACE_TRACE ("ACE_Process_Manager::find_proc");

  for (size_t i = 0; i < this->current_count_; ++i)
    if (pid == this->process_table_[i].process_->getpid ())
      return i;

  return -1;
}

#if defined (ACE_WIN32)
// Locate the index in the table associated with <h>.  Must be
// called with the lock held.

ssize_t
ACE_Process_Manager::find_proc (ACE_HANDLE h)
{
  ACE_TRACE ("ACE_Process_Manager::find_proc");

  for (size_t i = 0; i < this->current_count_; ++i)
    if (h == this->process_table_[i].process_->gethandle ())
      return i;

  return -1;
}
#endif /* ACE_WIN32 */

// Wait for all the Processs to exit, or until <timeout> elapses.
// Returns the number of Processes remaining, or -1 on an error.

int
ACE_Process_Manager::wait (const ACE_Time_Value &timeout)
{
  ACE_TRACE ("ACE_Process_Manager::wait");

  ACE_Time_Value until = timeout;
  ACE_Time_Value remaining = timeout;

  if (until < ACE_Time_Value::max_time)
    until += ACE_OS::gettimeofday ();

  while (current_count_ > 0)
    {
      pid_t pid = this->wait (0, remaining);

      if (pid == ACE_INVALID_PID)       // wait() failed
        return -1;
      else if (pid == 0)     // timeout
        break;

      remaining = until < ACE_Time_Value::max_time
        ? until - ACE_OS::gettimeofday ()
        : ACE_Time_Value::max_time;

      if (remaining <= ACE_Time_Value::zero)
        break;

      // else Process terminated...wait for more...
    }
  return current_count_;
}

// Collect a single child process' exit status.  Store the exit code
// in *<stat_loc> if non-zero.  Call the appropriate exit_notify.  If
// <pid> == 0, wait for any of the Process_Manager's children (or as
// near as possible -- on Unix, we might accidentally get some other
// Process_Manager's Process, or an unmanaged Process, or a child
// process started by some other means.

pid_t
ACE_Process_Manager::wait (pid_t pid,
                           ACE_exitcode *status)
{
  ACE_TRACE ("ACE_Process_Manager::wait");

  return this->wait (pid,
                     ACE_Time_Value::max_time,
                     status);
}

// Collect a single child processes' exit status, unless <timeout>
// elapses before the process exits.  Same caveats about accidental
// Process reaping on Unix as above.

pid_t
ACE_Process_Manager::wait (pid_t pid,
                           const ACE_Time_Value &timeout,
                           ACE_exitcode *status)
{
  ACE_TRACE ("ACE_Process_Manager::wait");

  ACE_exitcode local_stat = 0;
  if (status == 0)
    status = &local_stat;

  *status = 0;

  ssize_t idx = -1;
  ACE_Process *proc = 0;

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (pid != 0)
    {
      idx = this->find_proc (pid);
      if (idx == -1)
        return ACE_INVALID_PID;
      else
        proc = process_table_[idx].process_;
    }

  if (proc != 0)
    pid = proc->wait (timeout, status);
  else
    {
      // Wait for any Process spawned by this Process_Manager.
#if defined (ACE_WIN32)
      HANDLE *handles;

      ACE_NEW_RETURN (handles,
                      HANDLE[current_count_],
                      ACE_INVALID_PID);

      for (size_t i = 0;
           i < current_count_;
           ++i)
        handles[i] =
          process_table_[i].process_->gethandle ();

      DWORD result = ::WaitForMultipleObjects (current_count_,
                                               handles,
                                               FALSE,
                                               timeout == ACE_Time_Value::max_time
                                               ? INFINITE
                                               : timeout.msec ());
      if (result == WAIT_FAILED)
        pid = ACE_INVALID_PID;
      else if (result == WAIT_TIMEOUT)
        pid = 0;
      else
        {
          ACE_ASSERT (result >= WAIT_OBJECT_0
                      && result < WAIT_OBJECT_0 + current_count_);

          idx = this->find_proc (handles[result - WAIT_OBJECT_0]);

          if (idx != -1)
            {
              pid = process_table_[idx].process_->getpid ();
              result = ::GetExitCodeProcess (handles[result - WAIT_OBJECT_0],
                                             status);
              if (result == 0)
                {
                  // <GetExitCodeProcess> failed!
                  this->remove_proc (idx);
                  pid = ACE_INVALID_PID;
                }
            }
          else
            {
              // uh oh...handle removed from process_table_, even though
              // we're holding a lock!
              delete [] handles;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("Process removed")
                                 ACE_TEXT (" -- somebody's ignoring the lock!\n")),
                                -1);
            }
        }

      delete [] handles;
#else /* !defined(ACE_WIN32) */
      if (timeout == ACE_Time_Value::max_time)
        pid = ACE_OS::waitpid (-(ACE_OS::getpid ()),
                               status,
                               0);
      else if (timeout == ACE_Time_Value::zero)
        pid = ACE_OS::waitpid (-(ACE_OS::getpid ()),
                               status,
                               WNOHANG);
      else
        {
          ACE_Time_Value wait_until =
            timeout + ACE_OS::gettimeofday();

          for (;;)
            {
              pid = ACE_OS::waitpid (-(ACE_OS::getpid()),
                                     status,
                                     WNOHANG);
              if (pid != 0)
                // "no such children" error, or got one!
                break;

              ACE_Sig_Set alarm_or_child;

              alarm_or_child.sig_add (SIGALRM);
              alarm_or_child.sig_add (SIGCHLD);

              ACE_Time_Value time_left = wait_until - ACE_OS::gettimeofday ();

              // if ACE_OS::ualarm doesn't have sub-second resolution:
              time_left += ACE_Time_Value (0, 500000);
              time_left.usec (0);

              if (time_left <= ACE_Time_Value::zero) {
                pid = 0;
                break;
              }

              ACE_OS::ualarm (time_left);
              ACE_OS::sigwait (alarm_or_child);
            }
        }
#endif /* !defined (ACE_WIN32) */
    }

  if (pid != ACE_INVALID_PID && pid != 0)
    {
      if (proc == 0)
        {
          idx = this->find_proc (pid);
          if (idx == -1)
            {
              // oops, reaped an unmanaged process!
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) oops, reaped unmanaged %d\n"),
                          pid));
              return pid;
            }
          else
            proc = process_table_[idx].process_;
        }
      else
        ACE_ASSERT (pid == proc->getpid ());

      this->notify_proc_handler (idx,
                                 *status);
      this->remove_proc (idx);
    }

  return pid;
}

// Legacy method:

int
ACE_Process_Manager::reap (pid_t pid,
                           ACE_exitcode *stat_loc,
                           int options)
{
  ACE_TRACE ("ACE_Process_Manager::reap");

  return this->wait (pid,
                     (ACE_BIT_ENABLED (options, WNOHANG)
                      ? ACE_Time_Value::zero
                      : ACE_Time_Value::max_time),
                     stat_loc);
}

// Notify either the process-specific handler or the generic handler.
// If process-specific, call handle_close on the handler.  Returns 1
// if process found, 0 if not.  Must be called with locks held.

int
ACE_Process_Manager::notify_proc_handler (size_t i,
                                          ACE_exitcode exit_code)
{
  if (i < current_count_)
    {
      ACE_Process_Descriptor &proc_desc =
        this->process_table_[i];

      proc_desc.process_->exit_code (exit_code);

      if (proc_desc.exit_notify_ != 0)
        proc_desc.exit_notify_->handle_exit (proc_desc.process_);
      else if (this->default_exit_handler_ != 0
               && this->default_exit_handler_->handle_exit (proc_desc.process_) < 0)
        {
          this->default_exit_handler_->handle_close
            (ACE_INVALID_HANDLE,
             0);
          this->default_exit_handler_ = 0;
        }
      return 1;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P:%t|%T) ACE_Process_Manager::notify_proc_handler:"),
                  ACE_TEXT (" unknown/unmanaged process reaped\n")));
      return 0;
    }
}
