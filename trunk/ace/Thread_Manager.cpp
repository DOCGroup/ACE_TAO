// Thread_Manager.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Thread_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Thread_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Control)
ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Manager)

void
ACE_Thread_Manager::dump (void) const
{
  ACE_TRACE ("ACE_Thread_Manager::dump");
}

ACE_Thread_Descriptor::ACE_Thread_Descriptor (void)
  : thr_id_ (ACE_OS::NULL_thread),
    grp_id_ (0),
    thr_state_ (ACE_THR_IDLE)
{
  ACE_TRACE ("ACE_Thread_Descriptor::ACE_Thread_Descriptor");
}

// The following macro simplifies subsequence code.
#define ACE_FIND(OP,INDEX) \
  int INDEX = OP; \
  if (INDEX == -1) return -1

int 
ACE_Thread_Manager::thread_descriptor (ACE_thread_t thr_id,
				       ACE_Thread_Descriptor &descriptor)
{
  ACE_TRACE ("ACE_Thread_Descriptor::thread_descriptor");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_FIND (this->find_thread (thr_id), index);
  descriptor = this->thr_table_[index];
  return 0;
}


int 
ACE_Thread_Manager::hthread_descriptor (ACE_hthread_t thr_handle, 
					ACE_Thread_Descriptor &descriptor)
{
  ACE_TRACE ("ACE_Thread_Descriptor::hthread_descriptor");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_FIND (this->find_hthread (thr_handle), index);
  descriptor = this->thr_table_[index];
  return 0;
}

// Return the thread descriptor (indexed by ACE_hthread_t).

int 
ACE_Thread_Manager::thr_self (ACE_hthread_t &self)
{
  ACE_TRACE ("ACE_Thread_Descriptor::thr_self");  
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  // Try to get the cached HANDLE out of TSS to avoid lookup.
  ACE_hthread_t *handle = ACE_LOG_MSG->thr_handle ();

  // Wasn't in the cache, so we'll have to look it up and cache it.
  if (handle == 0) 
    { 
      ACE_thread_t id = ACE_OS::thr_self ();
      
      ACE_FIND (this->find_thread (id), index);
      handle = &this->thr_table_[index].thr_handle_;

      // Update the TSS cache. 
      ACE_LOG_MSG->thr_handle (handle);
    } 
  self = *handle;
  return 0;
}

int
ACE_Thread_Manager::resize (size_t size)
{
  ACE_TRACE ("ACE_Thread_Manager::resize");
  ACE_Thread_Descriptor *temp;
  
  ACE_NEW_RETURN (temp, ACE_Thread_Descriptor[size], -1);

  for (size_t i = 0; i < this->max_table_size_; i++)
    temp[i] = this->thr_table_[i]; // Structure assignment.

  this->max_table_size_ = size;

  delete [] this->thr_table_;

  this->thr_table_ = temp;
  return 0;
}

// Create and initialize the table to keep track of the thread pool.

int
ACE_Thread_Manager::open (size_t size)
{
  ACE_TRACE ("ACE_Thread_Manager::open");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->max_table_size_ < size)
    this->resize (size);
  return 0;
}

// Initialize the synchronization variables.

ACE_Thread_Manager::ACE_Thread_Manager (size_t size)
  : thr_table_ (0),
    max_table_size_ (0), 
    current_count_ (0),
    grp_id_ (1)
#if defined (ACE_HAS_THREADS)
    , zero_cond_ (lock_)
#endif /* ACE_HAS_THREADS */
{
  ACE_TRACE ("ACE_Thread_Manager::ACE_Thread_Manager");
  if (this->open (size) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Thread_Manager"));
}

// Close up and release all resources.

int
ACE_Thread_Manager::close (void)
{
  ACE_TRACE ("ACE_Thread_Manager::close");
  if (this->thr_table_ != 0)
    {
      delete [] this->thr_table_;
      this->thr_table_ = 0;
      this->max_table_size_ = 0;
      this->current_count_ = 0;
    }
  return 0;
}

ACE_Thread_Manager::~ACE_Thread_Manager (void)
{
  ACE_TRACE ("ACE_Thread_Manager::~ACE_Thread_Manager");
  this->close ();
}

// Call the appropriate OS routine to spawn a thread.  Should *not* be
// called with the lock_ held...

int 
ACE_Thread_Manager::spawn_i (ACE_THR_FUNC func, 
			     void *args, 
			     long flags, 
			     ACE_thread_t *t_id, 
			     ACE_hthread_t *t_handle,
			     long priority,
			     int grp_id,
			     void *stack, 
			     size_t stack_size,
			     ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Thread_Manager::spawn_i");
  ACE_thread_t thr_id;
  ACE_hthread_t thr_handle;

  if (t_id == 0)
    t_id = &thr_id;

  if (t_handle == 0)
    t_handle = &thr_handle;
  
  int result = ACE_Thread::spawn (func, args, flags, 
				  t_id, t_handle, priority,
				  stack, stack_size);
  if (result != 0)
    {
      // _Don't_ clobber errno here!  result is either 0 or -1, and
      // ACE_OS::thr_create () already set errno!    D. Levine 28 Mar 1997
      // errno = result;
      return -1;
    }
  else
    return this->append_thr (*t_id, *t_handle, 
			     ACE_THR_SPAWNED, 
			     grp_id, task);
}

// Create a new thread running FUNC.  *Must* be called with the lock_
// held...

int 
ACE_Thread_Manager::spawn (ACE_THR_FUNC func, 
			   void *args, 
			   long flags, 
			   ACE_thread_t *t_id, 
			   ACE_hthread_t *t_handle,
			   long priority,
			   int grp_id,
			   void *stack, 
			   size_t stack_size)
{
  ACE_TRACE ("ACE_Thread_Manager::spawn");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (grp_id == -1)
    grp_id = this->grp_id_++; // Increment the group id.

  if (this->spawn_i (func, args, flags, t_id, t_handle, 
		     priority, grp_id, stack, stack_size) == -1)
    return -1;
  else
    return grp_id;
}

// Create N new threads running FUNC.

int 
ACE_Thread_Manager::spawn_n (int n, 
			     ACE_THR_FUNC func, 
			     void *args, 
			     long flags,
			     long priority,
			     int grp_id,
			     ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Thread_Manager::spawn_n");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (grp_id == -1)
    grp_id = this->grp_id_++; // Increment the group id.

  for (int i = 0; i < n; i++)
    {
      // @@ What should happen if this fails?! e.g., should we try to
      // cancel the other threads that we've already spawned or what?
      if (this->spawn_i (func, args, flags, 0, 0, priority, grp_id, 
			 0, 0, task) == -1)
	return -1;
    }

  return grp_id;
}

// Create N new threads running FUNC.

int 
ACE_Thread_Manager::spawn_n (ACE_thread_t thread_ids[],
			     size_t n, 
			     ACE_THR_FUNC func, 
			     void *args,
			     long flags, 
			     long priority,
			     int grp_id,
			     void *stack[],
			     size_t stack_size[],
			     ACE_hthread_t thread_handles[])
{
  ACE_TRACE ("ACE_Thread_Manager::spawn_n");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (grp_id == -1)
    grp_id = this->grp_id_++; // Increment the group id.

  for (size_t i = 0; i < n; i++)
    {
      // @@ What should happen if this fails?! e.g., should we try to
      // cancel the other threads that we've already spawned or what?
      if (this->spawn_i (func, 
			 args, 
			 flags, 
			 &thread_ids[i], 
			 thread_handles == 0 ? 0 : &thread_handles[i], 
			 priority, 
			 grp_id, 
			 stack == 0 ? 0 : stack[i], 
			 stack_size == 0 ? 0 : stack_size[i]) == -1)
	return -1;
    }

  return grp_id;
}

// Append a thread into the pool (does not check for duplicates).
// Must be called with locks held.

int
ACE_Thread_Manager::append_thr (ACE_thread_t t_id, 
				ACE_hthread_t t_handle,
				ACE_Thread_State thr_state,
				int grp_id,
				ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Thread_Manager::append_thr");
  // Try to resize the array to twice its existing size if we run out
  // of space...
  if (this->current_count_ >= this->max_table_size_ 
      && this->resize (this->max_table_size_ * 2) == -1)
    return -1;
  else
    {
      ACE_Thread_Descriptor &thr_desc = 
	this->thr_table_[this->current_count_];

      thr_desc.thr_id_ = t_id;
      thr_desc.thr_handle_ = t_handle;
      thr_desc.grp_id_ = grp_id;
      thr_desc.thr_state_ = thr_state;
      thr_desc.task_ = task;

      this->current_count_++;
      return 0;
    }
}

// Return the thread descriptor (indexed by ACE_hthread_t).

int 
ACE_Thread_Manager::find_hthread (ACE_hthread_t h_id)
{
  ACE_TRACE ("ACE_Thread_Descriptor::find_hthread");  

  for (size_t i = 0; i < this->current_count_; i++)
    if (ACE_OS::thr_cmp (h_id, this->thr_table_[i].thr_handle_))
      return i;

  return -1;
}

// Locate the index in the table associated with <t_id>.  Must be
// called with the lock held.

int 
ACE_Thread_Manager::find_thread (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::find_thread");

  for (size_t i = 0; i < this->current_count_; i++)
    if (ACE_OS::thr_equal (t_id, this->thr_table_[i].thr_id_))
      return i;

  return -1;    
}

// Insert a thread into the pool (checks for duplicates and doesn't
// allow them to be inserted twice).

int
ACE_Thread_Manager::insert_thr (ACE_thread_t t_id, 
				ACE_hthread_t t_handle,
				int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::insert_thr");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  // Check for duplicates and bail out if they're already
  // registered...
  if (this->find_thread (t_id) != -1)
    return -1;

  if (grp_id == -1)
    grp_id = this->grp_id_++;
    
  if (this->append_thr (t_id, t_handle, 
			ACE_THR_SPAWNED,
			grp_id) == -1)
    return -1;
  else
    return grp_id;
}

// Remove a thread from the pool.  Must be called with locks held.

void
ACE_Thread_Manager::remove_thr (int i)
{
  ACE_TRACE ("ACE_Thread_Manager::remove_thr");

  this->current_count_--;

  if (this->current_count_ > 0)
    // Compact the table by moving the last item into the slot vacated
    // by the index being removed (this is a structure assignment).
    this->thr_table_[i] = this->thr_table_[this->current_count_];

#if defined (ACE_HAS_THREADS)
  // Tell all waiters when there are no more threads left in the pool.
  if (this->current_count_ == 0)
    this->zero_cond_.broadcast ();
#endif /* ACE_HAS_THREADS */
}

// Factory out some common behavior to simplify the following methods.
#define ACE_THR_OP(OP,STATE) \
  int result = OP (this->thr_table_[i].thr_handle_); \
  if (result != 0) { \
    this->remove_thr (i); \
    errno = result; \
    return -1; \
  } \
  else { \
    this->thr_table_[i].thr_state_ = STATE; \
    return 0; \
  }

int
ACE_Thread_Manager::suspend_thr (int i)
{
  ACE_TRACE ("ACE_Thread_Manager::suspend_thr");

  ACE_THR_OP (ACE_Thread::suspend, ACE_THR_SUSPENDED);
}

int
ACE_Thread_Manager::resume_thr (int i)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_thr");

  ACE_THR_OP (ACE_Thread::resume, ACE_THR_RUNNING);
}

int
ACE_Thread_Manager::cancel_thr (int i)
{
  ACE_TRACE ("ACE_Thread_Manager::cancel_thr");
  this->thr_table_[i].thr_state_ = ACE_THR_CANCELLED;
  return 0;
}

int
ACE_Thread_Manager::kill_thr (int i, int signum)
{
  ACE_TRACE ("ACE_Thread_Manager::kill_thr");

  int result = ACE_Thread::kill ((ACE_thread_t) this->thr_table_[i].thr_id_,
				 signum);

  if (result != 0)
    { 
      // We need to save this across calls to remove_thr() since that
      // call may reset errno.
      int error = errno;

      this->remove_thr (i); 
      errno = error; 
      return -1; 
    } 
  else 
    return 0; 
}

#define ACE_EXECUTE_OP(OP) \
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1)); \
  ACE_FIND (this->find_thread (t_id), index); \
  return OP (index);

// Suspend a single thread.

int 
ACE_Thread_Manager::suspend (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::suspend");
  ACE_EXECUTE_OP (this->suspend_thr);
}

// Resume a single thread.

int 
ACE_Thread_Manager::resume (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::resume");
  ACE_EXECUTE_OP (this->resume_thr);
}

// Cancel a single thread.

int 
ACE_Thread_Manager::cancel (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::cancel");
  ACE_EXECUTE_OP (this->cancel_thr);
}

// Send a signal to a single thread.

int 
ACE_Thread_Manager::kill (ACE_thread_t t_id, int signum)
{
  ACE_TRACE ("ACE_Thread_Manager::kill");
  return ACE_Thread::kill (t_id, signum);
}

int
ACE_Thread_Manager::check_state (ACE_Thread_State state, 
				 ACE_thread_t id) 
{
  ACE_TRACE ("ACE_Thread_Manager::check_state");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_Thread_State *thr_state = 0;
#if 0
  int self_check = ACE_OS::thr_equal (id, ACE_OS::thr_self ());

  // If we're checking the state of our thread, try to get the cached
  // value out of TSS to avoid lookup.
  if (self_check) 
    thr_state = ACE_LOG_MSG->thr_state ();

  // Wasn't in the cache, so we'll have to look it up.
  if (thr_state == 0) 
    { 
      ACE_FIND (this->find_thread (id), index);
      thr_state = &this->thr_table_[index].thr_state_;

      if (self_check) // Update the TSS cache. 
	ACE_LOG_MSG->thr_state (thr_state); 
    } 
#else
   // Turn off caching for the time being until we figure out 
   // how to do it correctly in the face of deletions...
  ACE_FIND (this->find_thread (id), index);
  thr_state = &this->thr_table_[index].thr_state_;
#endif /* 0 */
  return *thr_state == state;
}

// Test if a single thread is suspended.

int 
ACE_Thread_Manager::testsuspend (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::testsuspend");
  return this->check_state (ACE_THR_SUSPENDED, t_id);
}

// Test if a single thread is active (i.e., resumed).

int 
ACE_Thread_Manager::testresume (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::testresume");
  return this->check_state (ACE_THR_RUNNING, t_id);
}

// Test if a single thread is cancelled.

int 
ACE_Thread_Manager::testcancel (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::testcancel");
  return this->check_state (ACE_THR_CANCELLED, t_id);
}

// Get group ids for a particular thread id.

int 
ACE_Thread_Manager::get_grp (ACE_thread_t t_id, int &grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::get_grp");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_FIND (this->find_thread (t_id), index);
  grp_id = this->thr_table_[index].grp_id_;
  return 0;
}

// Set group ids for a particular thread id.

int 
ACE_Thread_Manager::set_grp (ACE_thread_t t_id, int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::set_grp");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_FIND (this->find_thread (t_id), index);
  this->thr_table_[index].grp_id_ = grp_id;
  return 0;
}

// Suspend a group of threads.

int
ACE_Thread_Manager::apply_grp (int grp_id, 
			       THR_FUNC func,
			       int arg)
{
  ACE_TRACE ("ACE_Thread_Manager::apply_grp");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  int result = 0;

  for (size_t i = 0; i < this->current_count_; i++)
    if (this->thr_table_[i].grp_id_ == grp_id
	&& (this->*func)(i, arg) == -1)
      result = -1;

  return result;
}

int 
ACE_Thread_Manager::suspend_grp (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::suspend_grp");
  return this->apply_grp (grp_id, 
			  THR_FUNC (&ACE_Thread_Manager::suspend_thr));
}

// Resume a group of threads.

int 
ACE_Thread_Manager::resume_grp (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_grp");
  return this->apply_grp (grp_id, 
			  THR_FUNC (&ACE_Thread_Manager::resume_thr));
}

// Kill a group of threads.

int 
ACE_Thread_Manager::kill_grp (int grp_id, int signum)
{
  ACE_TRACE ("ACE_Thread_Manager::kill_grp");
  return this->apply_grp (grp_id, 
			  THR_FUNC (&ACE_Thread_Manager::kill_thr), signum);
}

// Cancel a group of threads.

int 
ACE_Thread_Manager::cancel_grp (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_grp");
  return this->apply_grp (grp_id, 
			  THR_FUNC (&ACE_Thread_Manager::cancel_thr));
}

int
ACE_Thread_Manager::apply_all (THR_FUNC func, int arg)
{
  ACE_TRACE ("ACE_Thread_Manager::apply_all");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  int result = 0;

  for (size_t i = 0; i < this->current_count_; i++)
    if ((this->*func)(i, arg) == -1)
      result = -1;

  return result;
}

// Resume all threads that are suspended.

int 
ACE_Thread_Manager::resume_all (void)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_all");
  return this->apply_all (THR_FUNC (&ACE_Thread_Manager::resume_thr));
}

int 
ACE_Thread_Manager::suspend_all (void)
{
  ACE_TRACE ("ACE_Thread_Manager::suspend_all");
  return this->apply_all (THR_FUNC (&ACE_Thread_Manager::suspend_thr));
}

int 
ACE_Thread_Manager::kill_all (int sig)
{
  ACE_TRACE ("ACE_Thread_Manager::kill_all");
  return this->apply_all (&ACE_Thread_Manager::kill_thr, sig);
}

int 
ACE_Thread_Manager::cancel_all (void)
{
  ACE_TRACE ("ACE_Thread_Manager::cancel_all");
  return this->apply_all (THR_FUNC (&ACE_Thread_Manager::cancel_thr));
}

// Must be called when thread goes out of scope to clean up its table
// slot.

void *
ACE_Thread_Manager::exit (void *status, int do_thr_exit)
{
  ACE_TRACE ("ACE_Thread_Manager::exit");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  int i = this->find_thread (ACE_Thread::self ());

  // Locate thread id.
  if (i != -1)
    this->remove_thr (i);

  if (do_thr_exit)
    {
      // Note, destructor is never called, so we must manually release
      // the lock...  Note that once we release the lock, it marks it
      // as being "free" so that the Guard's destructor won't try to
      // release it again.

      ACE_MT (ace_mon.release ()); 

      ACE_Thread::exit (status);
      // On reasonable systems ACE_Thread::exit() should not return.
      // However, due to horrible semantics with Win32 thread-specific
      // storage this call can return (don't ask...).
    }
  return 0;
}

// Wait for all the threads to exit.

int
ACE_Thread_Manager::wait (const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Thread_Manager::wait");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

#if defined (ACE_HAS_THREADS)
  while (this->current_count_ > 0)
    if (this->zero_cond_.wait (timeout) == -1)
      return -1;
#else
  ACE_UNUSED_ARG (timeout);
#endif /* ACE_HAS_THREADS */

  return 0;
}

// Wait for task  
int 
ACE_Thread_Manager::wait_task (ACE_Task_Base *task,
			       const ACE_Time_Value *timeout)
{
  // This method will be implemented in the future.  The way we
  // thought about it is by adding a linked list of pointers to
  // ACE_Condition_Thread_Mutex, where one will be allocated
  // dynaminclly when needed. Broadcasting the waiters will be done
  // in a similar manner to what's done today. When a thread is
  // removed (in remove_thr()) we'll check whether it is last in its
  // task and whether "somebody" is waiting for this task to end, if
  // so we'll broadcast the waiters.

  task = task;
  timeout = timeout;
  ACE_NOTSUP_RETURN (-1);
}

// Wait for group of threads
int 
ACE_Thread_Manager::wait_group (int grp_id, 
				const ACE_Time_Value *timeout)
{
  // This method will be implemented in the future. See the above
  // comment.
  grp_id = grp_id;
  timeout = timeout;
  ACE_NOTSUP_RETURN (-1);
}

int
ACE_Thread_Manager::apply_task (ACE_Task_Base *task, 
				THR_FUNC func,
				int arg)
{
  ACE_TRACE ("ACE_Thread_Manager::apply_task");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  int result = 0;

  for (size_t i = 0; i < this->current_count_; i++)
    if (this->thr_table_[i].task_ == task
	&& (this->*func)(i, arg) == -1)
      result = -1;

  return result;
}

// Suspend a task
int  
ACE_Thread_Manager::suspend_task (ACE_Task_Base *task)
{ 
  ACE_TRACE ("ACE_Thread_Manager::suspend_task");
  return this->apply_task (task, 
			   THR_FUNC (&ACE_Thread_Manager::suspend_thr));
}

// Resume a task.
int  
ACE_Thread_Manager::resume_task (ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_task");
  return this->apply_task (task, 
			   THR_FUNC (&ACE_Thread_Manager::resume_thr));
}

// Kill a task.
int  
ACE_Thread_Manager::kill_task (ACE_Task_Base *task, int /* signum */)
{
  ACE_TRACE ("ACE_Thread_Manager::kill_task");
  return this->apply_task (task, 
			   THR_FUNC (&ACE_Thread_Manager::kill_thr));
}

// Cancel a task.
int  
ACE_Thread_Manager::cancel_task (ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Thread_Manager::cancel_task");
  return this->apply_task (task, 
			   THR_FUNC (&ACE_Thread_Manager::cancel_thr));
}

// Locate the index in the table associated with <task> from the
// beginning of the table up to an index.  Must be called with the
// lock held.

int 
ACE_Thread_Manager::find_task (ACE_Task_Base *task, 
			       int index)
{
  ACE_TRACE ("ACE_Thread_Manager::find_task");

  if (index == -1)
      index = current_count_;

  for (int i = 0; i < index; i++)
    if (task == this->thr_table_[i].task_)
      return i;

  return -1;    
}

// Returns the number of ACE_Task in a group. 

int 
ACE_Thread_Manager::num_tasks_in_group (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::num_tasks_in_group");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  int tasks_count = 0;

  for (size_t i = 0; i < this->current_count_; i++)
    if (this->thr_table_[i].grp_id_ == grp_id
	&& this->find_task (thr_table_[i].task_, i) == -1)
      tasks_count++;

  return tasks_count;
}

// Returns the number of threads in an ACE_Task.

int 
ACE_Thread_Manager::num_threads_in_task (ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Thread_Manager::num_threads_in_task");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  int threads_count = 0;

  for (size_t i = 0; i < this->current_count_; i++)
    if (this->thr_table_[i].task_ == task)
      threads_count++;

  return threads_count;
}

// Returns in task_list a list of ACE_Tasks in a group.

int 
ACE_Thread_Manager::task_list (int grp_id, 
			       ACE_Task_Base *task_list[],
			       size_t n)
{
  ACE_TRACE ("ACE_Thread_Manager::task_list");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_Task_Base **task_list_iterator = task_list;
  size_t task_list_count = 0;

  for (size_t i = 0; i < this->current_count_; i++) 
    if (task_list_count < n
	&& this->thr_table_[i].grp_id_ == grp_id
	&& this->find_task (thr_table_[i].task_, i) == -1)
      {
	task_list_iterator[task_list_count] = this->thr_table_[i].task_;
	task_list_count++;
      }

  return 0;
}

// Returns in thread_list a list of thread ids in an ACE_Task.

int 
ACE_Thread_Manager::thread_list (ACE_Task_Base *task, 
				 ACE_thread_t thread_list[],
				 size_t n)
{
  ACE_TRACE ("ACE_Thread_Manager::thread_list");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_thread_t *thread_list_iterator = thread_list;
  size_t thread_list_count = 0;

  for (size_t i = 0; i < this->current_count_; i++)
    if (thread_list_count < n && this->thr_table_[i].task_ == task)
      {
	thread_list_iterator[thread_list_count] = this->thr_table_[i].thr_id_;
	thread_list_count++;
      }

  return 0;
}

// Returns in thread_list a list of thread handles in an ACE_Task.

int 
ACE_Thread_Manager::hthread_list (ACE_Task_Base *task, 
				  ACE_hthread_t hthread_list[],
				  size_t n)
{
  ACE_TRACE ("ACE_Thread_Manager::thread_list");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_hthread_t *hthread_list_iterator = hthread_list;
  size_t hthread_list_count = 0;

  for (size_t i = 0; i < this->current_count_; i++)
    if (this->thr_table_[i].task_ == task 
	&& hthread_list_count < n)
      {
	hthread_list_iterator[hthread_list_count] = 
	  this->thr_table_[i].thr_handle_;
	hthread_list_count++;
      }

  return 0;
}

int 
ACE_Thread_Manager::set_grp (ACE_Task_Base *task, int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::set_grp");

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  for (size_t i = 0; i < this->current_count_; i++)
    if (this->thr_table_[i].task_ == task)
      this->thr_table_[i].grp_id_ = grp_id;
  return 0;
}

int 
ACE_Thread_Manager::get_grp (ACE_Task_Base *task, int &grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::get_grp");

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_FIND (this->find_task (task), index);
  grp_id = this->thr_table_[index].grp_id_;
  return 0;
}

void
ACE_Thread_Control::dump (void) const
{
  ACE_TRACE ("ACE_Thread_Control::dump");
}

int
ACE_Thread_Control::insert (ACE_Thread_Manager *tm)
{
  ACE_TRACE ("ACE_Thread_Control::insert");
 
  ACE_hthread_t t_id;
  ACE_Thread::self (t_id);
  this->tm_ = tm;
 
  return this->tm_->insert_thr (ACE_Thread::self (), t_id);
}

// Initialize the thread controller.

ACE_Thread_Control::ACE_Thread_Control (ACE_Thread_Manager *t, 
					int insert)
  : tm_ (t),
    status_ (0)
{
  ACE_TRACE ("ACE_Thread_Control::ACE_Thread_Control");
  if (this->tm_ != 0 && insert)
    {
      ACE_hthread_t t_id;
      ACE_Thread::self (t_id);
      this->tm_->insert_thr (ACE_Thread::self (), t_id);
    }
}

// Automatically kill thread on exit.

ACE_Thread_Control::~ACE_Thread_Control (void)
{
  ACE_TRACE ("ACE_Thread_Control::~ACE_Thread_Control");
#if defined (ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS)
  this->exit (this->status_, 0);
#else
  this->exit (this->status_, 1);
#endif /* ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS */
}

// Exit from thread (but clean up first).

void *
ACE_Thread_Control::exit (void *exit_status, int do_thr_exit)
{
  ACE_TRACE ("ACE_Thread_Control::exit");

  if (this->tm_ != 0)
    return this->tm_->exit (exit_status, do_thr_exit);
  else
    {
      ACE_Thread::exit (exit_status);
      return 0;
    }
}
