// Thread_Manager.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Synch_T.h"
#include "ace/Thread_Manager.h"
#include "ace/Dynamic.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Thread_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Control)
ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Manager)

// Process-wide Thread Manager.
ACE_Thread_Manager *ACE_Thread_Manager::thr_mgr_ = 0;

// Controls whether the Thread_Manager is deleted when we shut down
// (we can only delete it safely if we created it!)
int ACE_Thread_Manager::delete_thr_mgr_ = 0;

void
ACE_Thread_Manager::dump (void) const
{
  ACE_TRACE ("ACE_Thread_Manager::dump");
  // Cast away const-ness of this in order to use its non-const lock_.
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon,
                     ((ACE_Thread_Manager *) this)->lock_));

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, "\ngrp_id_ = %d", this->grp_id_));
  ACE_DEBUG ((LM_DEBUG, "\nmax_table_size_ = %d", this->max_table_size_));
  ACE_DEBUG ((LM_DEBUG, "\ncurrent_count_ = %d", this->current_count_));

  for (size_t i = 0; i < this->current_count_; i++)
    this->thr_table_[i].dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

int
ACE_Thread_Descriptor::at_exit (void *object,
                                ACE_CLEANUP_FUNC cleanup_hook,
                                void *param)
{
  ACE_TRACE ("ACE_Thread_Descriptor::at_exit");

  // @@ This should really store these values into a stack, but we're
  // just solving one problem at a time now...
  this->cleanup_info_.object_ = object;
  this->cleanup_info_.cleanup_hook_ = cleanup_hook;
  this->cleanup_info_.param_ = param;
  return 0;
}

void
ACE_Thread_Descriptor::dump (void) const
{
  ACE_TRACE ("ACE_Thread_Descriptor::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, "\nthr_id_ = %d", this->thr_id_));
  ACE_DEBUG ((LM_DEBUG, "\nthr_handle_ = %d", this->thr_handle_));
  ACE_DEBUG ((LM_DEBUG, "\ngrp_id_ = %d", this->grp_id_));
  ACE_DEBUG ((LM_DEBUG, "\nthr_state_ = %d", this->thr_state_));
  ACE_DEBUG ((LM_DEBUG, "\ncleanup_info_.cleanup_hook_ = %x", this->cleanup_info_.cleanup_hook_));
  ACE_DEBUG ((LM_DEBUG, "\nflags_ = %x\n", this->flags_));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Thread_Descriptor::ACE_Thread_Descriptor (void)
  : task_ (0),
    thr_id_ (ACE_OS::NULL_thread),
    thr_handle_ (ACE_OS::NULL_hthread),
    grp_id_ (0),
    thr_state_ (ACE_THR_IDLE)
{
  ACE_TRACE ("ACE_Thread_Descriptor::ACE_Thread_Descriptor");

  this->cleanup_info_.cleanup_hook_ = 0;
  this->cleanup_info_.object_ = 0;
  this->cleanup_info_.param_ = 0;
}

// The following macro simplifies subsequence code.
#define ACE_FIND(OP,INDEX) \
  int INDEX = OP; \
  if (INDEX == -1) return -1

int 
ACE_Thread_Manager::thread_descriptor (ACE_thread_t thr_id,
                                       ACE_Thread_Descriptor &descriptor)
{
  ACE_TRACE ("ACE_Thread_Manager::thread_descriptor");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_FIND (this->find_thread (thr_id), index);
  descriptor = this->thr_table_[index];
  return 0;
}


int 
ACE_Thread_Manager::hthread_descriptor (ACE_hthread_t thr_handle, 
                                        ACE_Thread_Descriptor &descriptor)
{
  ACE_TRACE ("ACE_Thread_Manager::hthread_descriptor");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_FIND (this->find_hthread (thr_handle), index);
  descriptor = this->thr_table_[index];
  return 0;
}

// Return the thread descriptor (indexed by ACE_hthread_t).

int 
ACE_Thread_Manager::thr_self (ACE_hthread_t &self)
{
  ACE_TRACE ("ACE_Thread_Manager::thr_self");  
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

ACE_Thread_Manager *
ACE_Thread_Manager::instance (void)
{
  ACE_TRACE ("ACE_Thread_Manager::instance");

  if (ACE_Thread_Manager::thr_mgr_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));

      if (ACE_Thread_Manager::thr_mgr_ == 0)
        {
          ACE_NEW_RETURN (ACE_Thread_Manager::thr_mgr_, ACE_Thread_Manager, 0);
          ACE_Thread_Manager::delete_thr_mgr_ = 1;
        }
    }

  return ACE_Thread_Manager::thr_mgr_;
}

ACE_Thread_Manager *
ACE_Thread_Manager::instance (ACE_Thread_Manager *tm)
{
  ACE_TRACE ("ACE_Thread_Manager::instance");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                            *ACE_Static_Object_Lock::instance (), 0));

  ACE_Thread_Manager *t = ACE_Thread_Manager::thr_mgr_;
  // We can't safely delete it since we don't know who created it!
  ACE_Thread_Manager::delete_thr_mgr_ = 0;

  ACE_Thread_Manager::thr_mgr_ = tm;
  return t;
}

void
ACE_Thread_Manager::close_singleton (void)
{
  ACE_TRACE ("ACE_Thread_Manager::close_singleton");

  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon,
                     *ACE_Static_Object_Lock::instance ()));

  if (ACE_Thread_Manager::delete_thr_mgr_)
    {
      delete ACE_Thread_Manager::thr_mgr_;
      ACE_Thread_Manager::thr_mgr_ = 0;
      ACE_Thread_Manager::delete_thr_mgr_ = 0;
    }
}

// Close up and release all resources.

int
ACE_Thread_Manager::close (void)
{
  ACE_TRACE ("ACE_Thread_Manager::close");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

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

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
// Lock the creation of the Singleton.
ACE_Thread_Mutex ACE_Thread_Exit::ace_thread_exit_lock_;
#endif /* defined (ACE_MT_SAFE) */

#if defined (ACE_HAS_SIG_C_FUNC)
extern "C" void
ACE_Thread_Exit_cleanup (void *instance, void *)
{
  ACE_TRACE ("ACE_Thread_Exit::cleanup");

  delete (ACE_TSS_TYPE (ACE_Thread_Exit) *) instance;
}
#else
void
ACE_Thread_Exit::cleanup (void *instance, void *)
{
  ACE_TRACE ("ACE_Thread_Exit::cleanup");

  delete (ACE_TSS_TYPE (ACE_Thread_Exit) *) instance;
}
#endif /* ACE_HAS_SIG_C_FUNC */

// NOTE: this preprocessor directive should match the one in
// ACE_Task_Base::svc_run () below.  This prevents the two statics
// from being defined.
ACE_Thread_Exit *
ACE_Thread_Exit::instance (void)
{
#if (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)) && ! defined (ACE_HAS_PTHREAD_SIGMASK) && !defined (ACE_HAS_FSU_PTHREADS)
  ACE_TRACE ("ACE_Thread_Exit::instance");

  // Determines if we were dynamically allocated.
  static ACE_TSS_TYPE (ACE_Thread_Exit) *instance_;

  // Implement the Double Check pattern.

  if (instance_ == 0)
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ace_thread_exit_lock_, 0));

      if (instance_ == 0)
        {
          ACE_NEW_RETURN (instance_, ACE_TSS_TYPE (ACE_Thread_Exit), 0);

          // Register for destruction with ACE_Object_Manager.
#if defined ACE_HAS_SIG_C_FUNC
          ACE_Object_Manager::at_exit (instance_, ACE_Thread_Exit_cleanup, 0);
#else
          ACE_Object_Manager::at_exit (instance_, ACE_Thread_Exit::cleanup, 0);
#endif /* ACE_HAS_SIG_C_FUNC */
        }
    }

  return ACE_TSS_GET (instance_, ACE_Thread_Exit);
#else
  return 0;
#endif /* (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION) && ! ACE_HAS_PTHREAD_SIGMASK && ! ACE_HAS_FSU_PTHREADS */
}

// Grab hold of the Task * so that we can close() it in the
// destructor.

ACE_Thread_Exit::ACE_Thread_Exit (void)
  : status_ ((void *) -1)
{
  ACE_TRACE ("ACE_Thread_Exit::ACE_Thread_Exit");
}

// Set the this pointer...

void
ACE_Thread_Exit::thr_mgr (ACE_Thread_Manager *tm)
{
  ACE_TRACE ("ACE_Thread_Exit::thr_mgr");

  if (tm != 0)
    this->thread_control_.insert (tm);
}

// Set the thread exit status value.

void *
ACE_Thread_Exit::status (void *s)
{
  ACE_TRACE ("ACE_Thread_Exit::status");
  return this->status_ = s;
}

void *
ACE_Thread_Exit::status (void)
{
  ACE_TRACE ("ACE_Thread_Exit::status");
  return this->status_;
}

// When this object is destroyed the Task is automatically closed
// down!

ACE_Thread_Exit::~ACE_Thread_Exit (void)
{
  ACE_TRACE ("ACE_Thread_Exit::~ACE_Thread_Exit");
}

// Run the entry point for thread spawned under the control of the
// <ACE_Thread_Manager>.  This must be an extern "C" to make certain
// compilers happy...
//
// The interaction with <ACE_Thread_Exit> and
// <ace_thread_manager_adapter> works like this, with
// ACE_HAS_THREAD_SPECIFIC_STORAGE or ACE_HAS_TSS_EMULATION:
//
// o Every thread in the <ACE_Thread_Manager> is run with
//   <ace_thread_manager_adapter>.
//
// o <ace_thread_manager_adapter> retrieves the singleton
//   <ACE_Thread_Exit> instance from <ACE_Thread_Exit::instance>.
//   The singleton gets created in thread-specific storage
//   in the first call to that function.  The key point is that the
//   instance is in thread-specific storage.
//
// o A thread can exit by various means, such as <ACE_Thread::exit>, C++
//   or Win32 exception, "falling off the end" of the thread entry
//   point function, etc.
//
// o If you follow this so far, now it gets really fun . . .
//   When the thread-specific storage (for the thread that
//   is being destroyed) is cleaned up, the OS threads package (or
//   the ACE emulation of thread-specific storage) will destroy any
//   objects that are in thread-specific storage.  It has a list of
//   them, and just walks down the list and destroys each one.
//
// o That's where the ACE_Thread_Exit destructor gets called.

extern "C" void *
ace_thread_manager_adapter (void *args)
{
#if defined (ACE_HAS_TSS_EMULATION)
  // As early as we can in the execution of the new thread, allocate
  // its local TS storage.  Allocate it on the stack, to save dynamic
  // allocation/dealloction.
  void *ts_storage[ACE_TSS_Emulation::ACE_TSS_THREAD_KEYS_MAX];
  ACE_TSS_Emulation::tss_open (ts_storage);
#endif /* ACE_HAS_TSS_EMULATION */

  ACE_Thread_Adapter *thread_args = (ACE_Thread_Adapter *) args;

  // NOTE: this preprocessor directive should match the one in above 
  // ACE_Thread_Exit::instance ().  With the Xavier Pthreads package,
  // the exit_hook in TSS causes a seg fault.  So, this works around
  // that by creating exit_hook on the stack.
#if (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)) && ! defined (ACE_HAS_PTHREAD_SIGMASK) && !defined (ACE_HAS_FSU_PTHEADS)
  // Obtain our thread-specific exit hook and make sure that it knows
  // how to clean us up!  Note that we never use this pointer directly
  // (it's stored in thread-specific storage), so it's ok to
  // dereference it here and only store it as a reference.
  ACE_Thread_Exit &exit_hook = *ACE_Thread_Exit::instance ();
#else
  // Without TSS, create an <ACE_Thread_Exit> instance.  When this
  // function returns, its destructor will be called because the
  // object goes out of scope.  The drawback with this appraoch is
  // that the destructor _won't_ get called if <thr_exit> is called.
  // So, threads shouldn't exit that way.  Instead, they should return
  // from <svc>.
  ACE_Thread_Exit exit_hook;
#endif /* (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION) && ! ACE_HAS_PTHREAD_SIGMASK && !ACE_HAS_FSU_PTHREADS */

  // Keep track of the <Thread_Manager> that's associated with this
  // <exit_hook>.
  exit_hook.thr_mgr (thread_args->thr_mgr ());

  // Invoke the user-supplied function with the args.
  void *status = thread_args->invoke ();

#if defined (ACE_HAS_TSS_EMULATION)
  // Lastly, close the thread's local TS storage.
  ACE_TSS_Emulation::tss_close (ts_storage);
#endif /* ACE_HAS_TSS_EMULATION */

  return status;
}

// Call the appropriate OS routine to spawn a thread.  Should *not* be
// called with the lock_ held...

// @@ Consider duping the handles so that users can have something
// to manipulate if they want.

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
  ACE_Thread_Adapter *thread_args = 0;
#if !defined (ACE_NO_THREAD_ADAPTER)
  ACE_NEW_RETURN (thread_args,
                  ACE_Thread_Adapter (func,
                                      args,
                                      (ACE_THR_C_FUNC) ace_thread_manager_adapter,
                                      this),
                  -1);
#endif /* ACE_NO_THREAD_ADAPTER */

  ACE_TRACE ("ACE_Thread_Manager::spawn_i");
  ACE_thread_t thr_id;
  ACE_hthread_t thr_handle;

  if (t_id == 0)
    t_id = &thr_id;

  int result = ACE_Thread::spawn (func,
                                  args,
                                  flags,
                                  t_id,
                                  &thr_handle,
                                  priority,
                                  stack,
                                  stack_size,
                                  thread_args);
  if (result != 0)
    // _Don't_ clobber errno here!  result is either 0 or -1, and
    // ACE_OS::thr_create () already set errno!  D. Levine 28 Mar 1997
    // errno = result;
    return -1;
  else
    {
#if defined (ACE_HAS_WTHREADS)
      // Have to duplicate handle if client asks for it.
      if (t_handle != 0)
        (void) ::DuplicateHandle (::GetCurrentProcess (), 
                                  thr_handle,
                                  ::GetCurrentProcess (),
                                  t_handle,
                                  0, 
                                  TRUE,
                                  DUPLICATE_SAME_ACCESS);
#endif /* ACE_HAS_WTHREADS */
      
      return this->append_thr (*t_id,
                               thr_handle, 
                               ACE_THR_SPAWNED, 
                               grp_id,
                               task,
                               flags);
    }
}

// Create a new thread running <func>.  *Must* be called with the
// <lock_> held...

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
ACE_Thread_Manager::spawn_n (size_t n, 
                             ACE_THR_FUNC func, 
                             void *args, 
                             long flags,
                             long priority,
                             int grp_id,
                             ACE_Task_Base *task,
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
                         0,
                         thread_handles == 0 ? 0 : &thread_handles[i], 
                         priority,
                         grp_id, 
                         0,
                         0,
                         task) == -1)
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
                         thread_ids == 0 ? 0 : &thread_ids[i], 
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
                                ACE_Task_Base *task,
				long flags)
{
  ACE_TRACE ("ACE_Thread_Manager::append_thr");
  // @@ This code will need to be replaced with a loop that will
  // iterate from 0 to this->max_table_size_ looking for a
  // thr_table_[i].thr_state_ entry that is set to ACE_THR_IDLE.  Only
  // if all the entries are in use do we have to reallocate the table.
  // Note that at some point we should use an "in situ" free list that
  // is woven through the table...

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
      thr_desc.cleanup_info_.cleanup_hook_ = 0;
      thr_desc.cleanup_info_.object_ = 0;
      thr_desc.cleanup_info_.param_ = 0;
      thr_desc.flags_ = flags;

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
                                int grp_id,
				long flags)
{
  ACE_TRACE ("ACE_Thread_Manager::insert_thr");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  // Check for duplicates and bail out if we're already registered...
  if (this->find_thread (t_id) != -1)
    return -1;

  if (grp_id == -1)
    grp_id = this->grp_id_++;
    
  if (this->append_thr (t_id,
			t_handle, 
                        ACE_THR_SPAWNED,
                        grp_id,
			0,
			flags) == -1)
    return -1;
  else
    return grp_id;
}

// Run the registered hooks when the thread exits.

void
ACE_Thread_Manager::run_thread_exit_hooks (int i)
{
#if 0 // currently unused!
  ACE_TRACE ("ACE_Thread_Manager::run_thread_exit_hooks");

  // @@ Currently, we have just one hook.  This should clearly be
  // generalized to support an arbitrary number of hooks.

  if (this->thr_table_[i].cleanup_info_.cleanup_hook_ != 0)
    {
      (*this->thr_table_[i].cleanup_info_.cleanup_hook_) 
        (this->thr_table_[i].cleanup_info_.object_,
         this->thr_table_[i].cleanup_info_.param_);

      this->thr_table_[i].cleanup_info_.cleanup_hook_ = 0;
    }
#else
  ACE_UNUSED_ARG (i);
#endif // 0
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

// Factor out some common behavior to simplify the following methods.
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
ACE_Thread_Manager::join_thr (int i)
{
  ACE_TRACE ("ACE_Thread_Manager::join_thr");

  int result = ACE_Thread::join (this->thr_table_[i].thr_handle_);
  if (result != 0)
    {
      this->remove_thr (i);
      errno = result;
      return -1;
    }
  return 0;
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
ACE_Thread_Manager::kill_thr (int i, int arg)
{
  ACE_TRACE ("ACE_Thread_Manager::kill_thr");

  int signum = (int) arg;

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

// Wait for group of threads

int 
ACE_Thread_Manager::wait_grp (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::wait_grp");

  int copy_count = 0;
  ACE_Thread_Descriptor *copy_table = 0;

  // We have to make sure that while we wait for these threads to
  // exit, we do not have the lock. Therefore we make a copy of all
  // interesting entries and let go of the lock.
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

    copy_table = new ACE_Thread_Descriptor [this->current_count_];  

    for (size_t i = 0; i < this->current_count_; i++)
      if (this->thr_table_[i].grp_id_ == grp_id)
        {
          copy_table[copy_count] = this->thr_table_[i];
          copy_count++;
        }
  }
  
  // Now to do the actual work
  int result = 0;

  for (int i = 0;
       i < copy_count && result != -1;
       i++)
    if (ACE_Thread::join (copy_table[i].thr_handle_) == -1)
      result = -1;
  
  delete [] copy_table;

  return result;
}

int 
ACE_Thread_Manager::at_exit (void *object,
                             ACE_CLEANUP_FUNC cleanup_hook,
                             void *param)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  // Locate thread id.
  int i = this->find_thread (ACE_Thread::self ());

  if (i != -1)
    return this->thr_table_[i].at_exit (object, cleanup_hook, param);
  else
    return -1;
}

// Must be called when thread goes out of scope to clean up its table
// slot.

void *
ACE_Thread_Manager::exit (void *status, int do_thr_exit)
{
  ACE_TRACE ("ACE_Thread_Manager::exit");

  int i;
  ACE_Cleanup_Info cleanup_info;

  // Just hold onto the guard while finding this thread's id and
  // copying the exit hook.
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

    // Locate thread id.
    i = this->find_thread (ACE_Thread::self ());

    if (i != -1)
      {
	// @@ Currently, we have just one hook.  This should clearly
	// be generalized to support an arbitrary number of hooks.

	if (this->thr_table_[i].cleanup_info_.cleanup_hook_ != 0)
	  {
	    // Copy the hook so that we can call it after releasing
	    // the guard.
	    cleanup_info = this->thr_table_[i].cleanup_info_;
	    this->thr_table_[i].cleanup_info_.cleanup_hook_ = 0;
	  }

        // @@ I still don't know how to distinguish threads that
        // fall off the end or exit normally from those that are
        // joined explicitly by user.  Hopefully, after we put
        // ACE_Thread_Descriptor addresses into TSS, we can find a
        // way to solve this.

#if !defined (VXWORKS)
	// Threads created with THR_DAEMON shouldn't exist here,
	// but just to be safe, let's put it here.
	if (((this->thr_table_[i].flags_ & (THR_DETACHED | THR_DAEMON)) == 0) ||
	    (this->thr_table_[i].flags_ & (THR_JOINABLE != 0)))
	  {
	    // Mark thread as terminated.
	    this->thr_table_[i].thr_state_ = ACE_THR_TERMINATED;
            this->terminated_thr_queue_.enqueue_tail
              (this->thr_table_[i]);
	  }
#endif /* VXWORKS */        

        // Remove thread descriptor from the table.
        this->remove_thr (i);
      }
    // Release the guard.
  }

  // Call the cleanup hook.
  if (cleanup_info.cleanup_hook_ != 0)
    (*cleanup_info.cleanup_hook_) (cleanup_info.object_,
				   cleanup_info.param_);

  if (do_thr_exit)
    {
      ACE_Thread::exit (status);
      // On reasonable systems <ACE_Thread::exit> should not return.
      // However, due to horrible semantics with Win32 thread-specific
      // storage this call can return (don't ask...).
    }

  return 0;
}

// Wait for all the threads to exit.

int
ACE_Thread_Manager::wait (const ACE_Time_Value *timeout)
{
  // @@ What we should do is build a table of threads which have been
  // removed so that we can ``join'' with them at this point and also
  // close down the handles.

  ACE_TRACE ("ACE_Thread_Manager::wait");

#if defined (ACE_HAS_THREADS)
  size_t threads_waited_on;

  // Just hold onto the guard while waiting.
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

    threads_waited_on = this->current_count_;

    while (this->current_count_ > 0)
      if (this->zero_cond_.wait (timeout) == -1)
        return -1;
  }

  // @@ Hopefully, we can get rid of all this stuff if we keep a list
  // of threads to join with...  In addition, we should be able to
  // close down the HANDLE at this point, as well, on NT.  Note that
  // we can also mark the thr_table_[entry] as ACE_THR_IDLE once we've
  // joined with it.

  // Let go of the guard, giving other threads a chance to run.

  // Yield (four times) for each thread that we had to wait on.  This
  // should give each of those threads a chance to clean up.  The
  // problem arises because the threads that signalled zero_cond_ may
  // not have had a chance to run after that, and therefore may not
  // have finished cleaning themselves up.  This isn't a guaranteed
  // fix, of course, but that would be very complicated.
  for (size_t i = 0; i < 4 * threads_waited_on; ++i)
    ACE_OS::thr_yield ();

  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
    ACE_Thread_Descriptor item;

    while (! this->terminated_thr_queue_.dequeue_head (item))
#if !defined (VXWORKS)
	ACE_Thread::join (item.thr_handle_)
#endif /* VXWORKS */
          ;
  }
#else
  ACE_UNUSED_ARG (timeout);
#endif /* ACE_HAS_THREADS */

  return 0;
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

// Wait for task  
int 
ACE_Thread_Manager::wait_task (ACE_Task_Base *task)
{
  int copy_count = 0;
  ACE_Thread_Descriptor *copy_table = 0;

  // We have to make sure that while we wait for these threads to
  // exit, we do not have the lock. Therefore we make a copy of all
  // interesting entries and let go of the lock.
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

    ACE_NEW_RETURN (copy_table,
                    ACE_Thread_Descriptor [this->current_count_],
                    -1);

    for (size_t i = 0; i < this->current_count_; i++)
      if (this->thr_table_[i].task_ == task)
        {
          copy_table[copy_count] = this->thr_table_[i];
          copy_count++;
        }
  }
  
  // Now to do the actual work
  int result = 0;

  for (int i = 0; i < copy_count && result != -1; i++)
    if (ACE_Thread::join (copy_table[i].thr_handle_) == -1)
      result = -1;
  
  delete [] copy_table;

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

#if defined (ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS) || defined (ACE_HAS_TSS_EMULATION)
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
#if !defined (ACE_HAS_TSS_EMULATION)
      // With ACE_HAS_TSS_EMULATION, we let ACE_Thread_Adapter::invoke ()
      // exit the thread after cleaning up TSS.
      ACE_Thread::exit (exit_status);
#endif /* ! ACE_HAS_TSS_EMULATION */
      return 0;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#if (defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))
  // This doesn't necessarily belong here, but it's a convenient place for it.
  template class ACE_TSS<ACE_Dynamic>;
  template class ACE_TSS<ACE_Thread_Exit>;
  template class ACE_Unbounded_Queue<ACE_Thread_Descriptor>;
  template class ACE_Node<ACE_Thread_Descriptor>;
#endif /* ACE_HAS_THREADS && (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION) */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if (defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))
  // This doesn't necessarily belong here, but it's a convenient place for it.
  #pragma instantiate ACE_TSS<ACE_Dynamic>
  #pragma instantiate ACE_TSS<ACE_Thread_Exit>
  #pragma instantiate ACE_Unbounded_Queue<ACE_Thread_Descriptor>
  #pragma instantiate ACE_Node<ACE_Thread_Descriptor>
#endif /* ACE_HAS_THREADS && (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION) */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
