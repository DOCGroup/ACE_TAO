// $Id$

#define ACE_BUILD_DLL
#include "ace/Synch_T.h"
#include "ace/Thread_Manager.h"
#include "ace/Dynamic.h"
#include "ace/Object_Manager.h"
#include "ace/Singleton.h"

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
ACE_Thread_Manager::dump (void)
{
  ACE_TRACE ("ACE_Thread_Manager::dump");
  // Cast away const-ness of this in order to use its non-const lock_.
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon,
                     ((ACE_Thread_Manager *) this)->lock_));

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, "\ngrp_id_ = %d", this->grp_id_));
  ACE_DEBUG ((LM_DEBUG, "\ncurrent_count_ = %d", this->thr_list_.size ()));

  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    iter.next ()->dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Thread_Descriptor::~ACE_Thread_Descriptor (void)
{
}

int
ACE_Thread_Descriptor::operator==(const ACE_Thread_Descriptor &rhs) const
{
  return ACE_OS::thr_cmp (this->thr_handle_, rhs.thr_handle_) == 0
    && ACE_OS::thr_equal (this->thr_id_, rhs.thr_id_) == 0;
}

int
ACE_Thread_Descriptor::operator!=(const ACE_Thread_Descriptor &rhs) const
{
  return !(*this == rhs);
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
  : thr_id_ (ACE_OS::NULL_thread),
    thr_handle_ (ACE_OS::NULL_hthread),
    grp_id_ (0),
    thr_state_ (ACE_THR_IDLE),
    task_ (0)
{
  ACE_TRACE ("ACE_Thread_Descriptor::ACE_Thread_Descriptor");

  this->cleanup_info_.cleanup_hook_ = 0;
  this->cleanup_info_.object_ = 0;
  this->cleanup_info_.param_ = 0;
}

// The following macro simplifies subsequence code.
#define ACE_FIND(OP,INDEX) \
  ACE_Thread_Descriptor *INDEX = OP; \

ACE_Thread_Descriptor *
ACE_Thread_Manager::thread_desc_self (void)
{
  // This method must be called with lock held.

  // Try to get it from cache.
  ACE_Thread_Descriptor *desc = ACE_LOG_MSG->thr_desc ();

  // Wasn't in the cache, so we'll have to look it up and cache it.
  // If ACE_HAS_THREAD_ADAPTER, we are guranteed never need to search
  // the list.
  if (desc == 0)
    {
      ACE_thread_t id = ACE_OS::thr_self ();

      desc = this->find_thread (id);

      // Thread descriptor adapter might not have been put into the
      // list yet.
      if (desc != 0)
        // Update the TSS cache.
        ACE_LOG_MSG->thr_desc (desc);
    }

  return desc;
}

ACE_Thread_Descriptor *
ACE_Thread_Manager::thread_descriptor (ACE_thread_t thr_id)
{
  ACE_TRACE ("ACE_Thread_Manager::thread_descriptor");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  ACE_FIND (this->find_thread (thr_id), ptr);
  return ptr;
}

ACE_Thread_Descriptor *
ACE_Thread_Manager::hthread_descriptor (ACE_hthread_t thr_handle)
{
  ACE_TRACE ("ACE_Thread_Manager::hthread_descriptor");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  ACE_FIND (this->find_hthread (thr_handle), ptr);
  return ptr;
}

// Return the thread descriptor (indexed by ACE_hthread_t).

int
ACE_Thread_Manager::thr_self (ACE_hthread_t &self)
{
  ACE_TRACE ("ACE_Thread_Manager::thr_self");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_Thread_Descriptor *desc =
    this->thread_desc_self ();

  if (desc == 0)
    return -1;
  else
    desc->self (self);

  return 0;
}

// Initialize the synchronization variables.

ACE_Thread_Manager::ACE_Thread_Manager (size_t)
  : grp_id_ (1),
    automatic_wait_ (1)
#if defined (ACE_HAS_THREADS)
    , zero_cond_ (lock_)
#endif /* ACE_HAS_THREADS */
{
  ACE_TRACE ("ACE_Thread_Manager::ACE_Thread_Manager");
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
      // First, we clean up the thread descriptor list.
      ACE_Thread_Manager::thr_mgr_->close ();
      delete ACE_Thread_Manager::thr_mgr_;
      ACE_Thread_Manager::thr_mgr_ = 0;
      ACE_Thread_Manager::delete_thr_mgr_ = 0;
    }
}

// Close up and release all resources.

int
ACE_Thread_Manager::close ()
{
  ACE_TRACE ("ACE_Thread_Manager::close");

  // Clean up the thread descriptor list.
  if (this->automatic_wait_)
    this->wait (0, 1);
  else
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

      this->remove_thr_all ();
    }

  return 0;
}

ACE_Thread_Manager::~ACE_Thread_Manager (void)
{
  ACE_TRACE ("ACE_Thread_Manager::~ACE_Thread_Manager");
  this->close ();
}

#if defined (ACE_HAS_SIG_C_FUNC)
extern "C" void
ACE_Thread_Exit_cleanup (void *instance, void *)
{
  ACE_TRACE ("ACE_Thread_Exit_cleanup");

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
#if (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)) && ! defined (ACE_HAS_PTHREAD_SIGMASK)
  ACE_TRACE ("ACE_Thread_Exit::instance");

  // Determines if we were dynamically allocated.
  static ACE_TSS_TYPE (ACE_Thread_Exit) *instance_;

  // Implement the Double Check pattern.

  if (instance_ == 0)
    {
      ACE_MT (ACE_Thread_Mutex *lock =
              ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
                (ACE_Object_Manager::ACE_THREAD_EXIT_LOCK);
              ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *lock, 0));

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
#endif /* (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION) && ! ACE_HAS_PTHREAD_SIGMASK */
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
#if (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)) && ! defined (ACE_HAS_PTHREAD_SIGMASK)
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
#endif /* (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION) && ! ACE_HAS_PTHREAD_SIGMASK */

  // Keep track of the <Thread_Manager> that's associated with this
  // <exit_hook>.
  exit_hook.thr_mgr (thread_args->thr_mgr ());

  // Invoke the user-supplied function with the args.
  void *status = thread_args->invoke ();

  return status;
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
  // First, threads created by Thread Manager should not be daemon threads.
  // Using assertion is probably a bit too strong.  However, it helps
  // finding this kind of error as early as possible.  Perhaps we can replace
  // assertion by returning error.
  ACE_ASSERT (ACE_BIT_DISABLED (flags, THR_DAEMON));

  // Create a new thread running <func>.  *Must* be called with the
  // <lock_> held...
  ACE_Thread_Descriptor *new_thr_desc = 0;

  ACE_NEW_RETURN (new_thr_desc,
                  ACE_Thread_Descriptor,
                  -1);

#if !defined (ACE_NO_THREAD_ADAPTER)
  // @@ Defining ACE_NO_THREAD_ADAPTER here really doesn't make any
  // sense.  If we don't have thread adapter, thread manager won't
  // work then.
  ACE_Thread_Adapter *thread_args =
    new ACE_Thread_Adapter (func,
                            args,
                            (ACE_THR_C_FUNC) ace_thread_manager_adapter,
                            this,
                            new_thr_desc);
  if (thread_args == 0)
    {
      delete new_thr_desc;
      return -1;
    }
#endif /* ACE_NO_THREAD_ADAPTER */

  ACE_TRACE ("ACE_Thread_Manager::spawn_i");
  ACE_hthread_t thr_handle;

#if defined (VXWORKS)
  // On VxWorks, ACE_thread_t is char *.
  if (t_id == 0)
    {
      char *thr_id;
      ACE_NEW_RETURN (thr_id, char[32], -1);
      ACE_OS::strcpy (thr_id, "==ace_t==");
      t_id = &thr_id;
    }
#else  /* ! VXWORKS */
  ACE_thread_t thr_id;
  if (t_id == 0)
    t_id = &thr_id;
#endif /* ! VXWORKS */

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
      // @@ How are thread handles implemented on AIX?  Do they
      // also need to be duplicated?
      if (t_handle != 0)
        (void) ::DuplicateHandle (::GetCurrentProcess (),
                                  thr_handle,
                                  ::GetCurrentProcess (),
                                  t_handle,
                                  0,
                                  TRUE,
                                  DUPLICATE_SAME_ACCESS);
#else
      ACE_UNUSED_ARG (t_handle);
#endif /* ACE_HAS_WTHREADS */

      // append_thr also put the <new_thr_desc> into Thread_Manager's
      // double-linked list.  Only after this point, can we manipulate
      // double-linked list from a spawned thread's context.
      return this->append_thr (*t_id,
                               thr_handle,
                               ACE_THR_SPAWNED,
                               grp_id,
                               task,
                               flags,
                               new_thr_desc);
    }
}

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
                             ACE_hthread_t thread_handles[],
                             void *stack[],
                             size_t stack_size[])
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
                         stack == 0 ? 0 : stack[i],
                         stack_size == 0 ? 0 : stack_size[i],
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
                                long flags,
                                ACE_Thread_Descriptor *td)
{
  ACE_TRACE ("ACE_Thread_Manager::append_thr");
  ACE_Thread_Descriptor *thr_desc;

  if (td == 0)
    ACE_NEW_RETURN (thr_desc, ACE_Thread_Descriptor, -1);
  else
    thr_desc = td;

  thr_desc->thr_id_ = t_id;
  thr_desc->thr_handle_ = t_handle;
  thr_desc->grp_id_ = grp_id;
  thr_desc->thr_state_ = thr_state;
  thr_desc->task_ = task;
  thr_desc->cleanup_info_.cleanup_hook_ = 0;
  thr_desc->cleanup_info_.object_ = 0;
  thr_desc->cleanup_info_.param_ = 0;
  thr_desc->flags_ = flags;

  this->thr_list_.insert_head (thr_desc);

  return 0;
}

// Return the thread descriptor (indexed by ACE_hthread_t).

ACE_Thread_Descriptor *
ACE_Thread_Manager::find_hthread (ACE_hthread_t h_id)
{
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    if (ACE_OS::thr_cmp (iter.next ()->thr_handle_, h_id))
      return iter.next ();

  return 0;
}

// Locate the index in the table associated with <t_id>.  Must be
// called with the lock held.

ACE_Thread_Descriptor *
ACE_Thread_Manager::find_thread (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::find_thread");

  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    if (ACE_OS::thr_equal (iter.next ()->thr_id_, t_id))
      return iter.next ();
  return 0;
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
#if defined (VXWORKS)
  if (this->find_hthread (t_handle) != 0 )
    return -1;
#else  /* ! VXWORKS */
  if (this->find_thread (t_id) != 0 )
    return -1;
#endif /* ! VXWORKS */

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

  ACE_Thread_Descriptor *td = this->thread_desc_self ();
  if (td != 0 && td->cleanup_info.cleanup_hook_ != 0)
    {
      (*td->cleanup_info_.cleanup_hook_)
        (td->cleanup_info_.object_,
         td->cleanup_info_.param_);

      td->cleanup_info_.cleanup_hook_ = 0;
    }
  ACE_UNUSED_ARG (i);
#else
  ACE_UNUSED_ARG (i);
#endif // 0
}

// Remove a thread from the pool.  Must be called with locks held.

void
ACE_Thread_Manager::remove_thr (ACE_Thread_Descriptor *td,
                                int close_handler)
{
  ACE_TRACE ("ACE_Thread_Manager::remove_thr");

#if defined (VXWORKS)
  ACE_thread_t tid = td->self ();
#endif /* VXWORKS */

  this->thr_list_.remove (td);

#if defined (VXWORKS)
  if (tid && ACE_OS::strncmp (tid, "==ace_t==", 9) == 0)
    {
      delete tid;
    }
#endif /* VXWORKS */

#if defined (ACE_WIN32)
  if (close_handler != 0)
    ::CloseHandle (td->thr_handle_);
#else
  ACE_UNUSED_ARG (close_handler);
#endif /* ACE_WIN32 */
  delete td;
#if defined (ACE_HAS_THREADS)
  // Tell all waiters when there are no more threads left in the pool.
  if (this->thr_list_.size () == 0)
    this->zero_cond_.broadcast ();
#endif /* ACE_HAS_THREADS */
}

// Repeatedly call remove_thr on all table entries until there
// is no thread left.   Must be called with lock held.

void
ACE_Thread_Manager::remove_thr_all (void)
{
  ACE_Thread_Descriptor *td;

  while ((td = this->thr_list_.delete_head ()) != 0)
    {
#if defined (ACE_WIN32)
      // We need to let go handles if we want to let the threads
      // run wild.
      // @@ Do we need to close down AIX thread handles too?
      ::CloseHandle (td->thr_handle_);
#endif /* ACE_WIN32 */
      delete td;
    }

}

// ------------------------------------------------------------------
// Factor out some common behavior to simplify the following methods.
#define ACE_THR_OP(OP,STATE) \
  int result = OP (td->thr_handle_); \
  if (result != 0) { \
    this->thr_to_be_removed_.enqueue_tail (td); \
    errno = result; \
    return -1; \
  } \
  else { \
    td->thr_state_ = STATE; \
    return 0; \
  }

int
ACE_Thread_Manager::join_thr (ACE_Thread_Descriptor *td, int)
{
  ACE_TRACE ("ACE_Thread_Manager::join_thr");

  int result = ACE_Thread::join (td->thr_handle_);
  if (result != 0)
    {
      // Since the thread are being joined, we should
      // let it remove itself from the list.

      //      this->remove_thr (td);
      errno = result;
      return -1;
    }
  return 0;
}

int
ACE_Thread_Manager::suspend_thr (ACE_Thread_Descriptor *td, int)
{
  ACE_TRACE ("ACE_Thread_Manager::suspend_thr");

  ACE_THR_OP (ACE_Thread::suspend, ACE_THR_SUSPENDED);
}

int
ACE_Thread_Manager::resume_thr (ACE_Thread_Descriptor *td, int)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_thr");

  ACE_THR_OP (ACE_Thread::resume, ACE_THR_RUNNING);
}

int
ACE_Thread_Manager::cancel_thr (ACE_Thread_Descriptor *td, int)
{
  ACE_TRACE ("ACE_Thread_Manager::cancel_thr");
  // @@ Don't really know how to handle thread cancel.
  td->thr_state_ = ACE_THR_CANCELLED;
  return 0;
}

int
ACE_Thread_Manager::kill_thr (ACE_Thread_Descriptor *td, int signum)
{
  ACE_TRACE ("ACE_Thread_Manager::kill_thr");

  ACE_thread_t tid = td->thr_id_;
#if defined (VXWORKS)
  tid += ACE_OS::strncmp (tid, "==ace_t==", 9) == 0  ?  9  :  0;
#endif /* VXWORKS */

  int result = ACE_Thread::kill (tid, signum);

  if (result != 0)
    {
      // Only remove a thread from us when there is a "real" error.
      if (errno != ENOTSUP)
        this->thr_to_be_removed_.enqueue_tail (td);

      return -1;
    }
  else
    return 0;
}

// ------------------------------------------------------------------
// Factor out some common behavior to simplify the following methods.
#define ACE_EXECUTE_OP(OP) \
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1)); \
  ACE_ASSERT (this->thr_to_be_removed_.is_empty ()); \
  ACE_FIND (this->find_thread (t_id), ptr); \
  int result = OP (ptr); \
  int error = errno; \
  while (! this->thr_to_be_removed_.is_empty ()) { \
    ACE_Thread_Descriptor *td; \
    this->thr_to_be_removed_.dequeue_head (td); \
    this->remove_thr (td, 1); \
  } \
  errno = error; \
  return result

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
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  ACE_ASSERT (this->thr_to_be_removed_.is_empty ());

  ACE_FIND (this->find_thread (t_id), ptr);
  int result = this->kill_thr (ptr, signum);
  int error = errno;
  while (! this->thr_to_be_removed_.is_empty ()) {
    ACE_Thread_Descriptor *td;
    this->thr_to_be_removed_.dequeue_head (td);
    this->remove_thr (td, 1);
  }
  errno = error;
  return result;
}

int
ACE_Thread_Manager::check_state (ACE_Thread_State state,
                                 ACE_thread_t id)
{
  ACE_TRACE ("ACE_Thread_Manager::check_state");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_Thread_State thr_state;

  int self_check = ACE_OS::thr_equal (id, ACE_OS::thr_self ());

  // If we're checking the state of our thread, try to get the cached
  // value out of TSS to avoid lookup.
  if (self_check)
    thr_state = this->thread_desc_self ()->thr_state_;
  else
    {
      // Not calling from self, have to look it up from the list.
      ACE_FIND (this->find_thread (id), ptr);
      if (ptr == 0)
        return 0;
      thr_state = ptr->thr_state_;
    }
  return thr_state == state;
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

  ACE_FIND (this->find_thread (t_id), ptr);
  grp_id = ptr->grp_id_;
  return 0;
}

// Set group ids for a particular thread id.

int
ACE_Thread_Manager::set_grp (ACE_thread_t t_id, int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::set_grp");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_FIND (this->find_thread (t_id), ptr);
  ptr->grp_id_ = grp_id;
  return 0;
}

// Suspend a group of threads.

int
ACE_Thread_Manager::apply_grp (int grp_id,
                               ACE_THR_MEMBER_FUNC func,
                               int arg)
{
  ACE_TRACE ("ACE_Thread_Manager::apply_grp");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_monx, this->lock_, -1));
  ACE_ASSERT (this->thr_to_be_removed_.is_empty ());

  int result = 0;

  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    if (iter.next ()->grp_id_ == grp_id)
      if ((this->*func) (iter.next (), arg) == -1)
        result = -1;

  // Must remove threads after we have traversed the thr_list_ to
  // prevent clobber thr_list_'s integrity.

  if (! this->thr_to_be_removed_.is_empty ())
    {
      // Preserve errno!
      int error = errno;
      ACE_Thread_Descriptor *td;
      while (this->thr_to_be_removed_.dequeue_head (td) != -1)
        this->remove_thr (td, 1);
      errno = error;
    }

  return result;
}

int
ACE_Thread_Manager::suspend_grp (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::suspend_grp");
  return this->apply_grp (grp_id,
                          ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::suspend_thr));
}

// Resume a group of threads.

int
ACE_Thread_Manager::resume_grp (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_grp");
  return this->apply_grp (grp_id,
                          ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::resume_thr));
}

// Kill a group of threads.

int
ACE_Thread_Manager::kill_grp (int grp_id, int signum)
{
  ACE_TRACE ("ACE_Thread_Manager::kill_grp");
  return this->apply_grp (grp_id,
                          ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::kill_thr), signum);
}

// Cancel a group of threads.

int
ACE_Thread_Manager::cancel_grp (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_grp");
  return this->apply_grp (grp_id,
                          ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::cancel_thr));
}

int
ACE_Thread_Manager::apply_all (ACE_THR_MEMBER_FUNC func, int arg)
{
  ACE_TRACE ("ACE_Thread_Manager::apply_all");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  ACE_ASSERT (this->thr_to_be_removed_.is_empty ());

  int result = 0;

  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    if ((this->*func)(iter.next (), arg) == -1)
      result = -1;

  // Must remove threads after we have traversed the thr_list_ to
  // prevent clobber thr_list_'s integrity.

  if (! this->thr_to_be_removed_.is_empty ())
    {
      // Preserve errno!
      int error = errno;
      ACE_Thread_Descriptor *td;
      while (this->thr_to_be_removed_.dequeue_head (td) != -1)
        this->remove_thr (td, 1);
      errno = error;
    }

  return result;
}

// Resume all threads that are suspended.

int
ACE_Thread_Manager::resume_all (void)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_all");
  return this->apply_all (ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::resume_thr));
}

int
ACE_Thread_Manager::suspend_all (void)
{
  ACE_TRACE ("ACE_Thread_Manager::suspend_all");
  return this->apply_all (ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::suspend_thr));
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
  return this->apply_all (ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::cancel_thr));
}

// Wait for group of threads

int
ACE_Thread_Manager::wait_grp (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::wait_grp");

  int copy_count = 0;
  ACE_Thread_Descriptor *copy_table = 0;

  // We have to make sure that while we wait for these threads to
  // exit, we do not have the lock.  Therefore we make a copy of all
  // interesting entries and let go of the lock.
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

    ACE_NEW_RETURN (copy_table, ACE_Thread_Descriptor [this->thr_list_.size ()], -1);

    for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
         !iter.done ();
         iter.advance ())
      // If threads are created as THR_DETACHED or THR_DAEMON, we
      // can't help much.
      if (iter.next ()->grp_id_ == grp_id &&
          (ACE_BIT_DISABLED (iter.next ()->flags_, (THR_DETACHED | THR_DAEMON))
           || ACE_BIT_ENABLED (iter.next ()->flags_, THR_JOINABLE)))
        copy_table[copy_count++] = *iter.next ();
  }

  // Now actually join() with all the threads in this group.
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

  return this->thread_desc_self ()->at_exit (object,
                                             cleanup_hook,
                                             param);
}

int
ACE_Thread_Manager::acquire_release (void)
{
  // Just try to acquire the lock then release it.
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon,
                            this->lock_, -1));
  return 0;
}

// Must be called when thread goes out of scope to clean up its table
// slot.

void *
ACE_Thread_Manager::exit (void *status, int do_thr_exit)
{
  ACE_TRACE ("ACE_Thread_Manager::exit");

#if defined (ACE_WIN32)
  if (do_thr_exit)
    {
      // On Win32, if we really wants to exit from a thread, we must
      // first  clean up the thread specific storage.  By doing so,
      // ACE_Thread_Manager::exit will be called again with
      // do_thr_exit = 0 and cleaning up the ACE_Cleanup_Info (but not
      // exiting the thread.)  After the following call returns, we
      // are safe to exit this thread.
      delete ACE_Thread_Exit::instance ();
      ACE_Thread::exit (status);
    }
#endif /* ACE_WIN32 */

  ACE_Cleanup_Info cleanup_info;

  // Just hold onto the guard while finding this thread's id and
  // copying the exit hook.
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

    // Find the thread id, but don't use the cache.  It might have been
    // deleted already.
#if defined (VXWORKS)
    ACE_hthread_t id;
    ACE_OS::thr_self (id);
    ACE_Thread_Descriptor *td = this->find_hthread (id);
#else  /* ! VXWORKS */
    ACE_thread_t id = ACE_OS::thr_self ();
    ACE_Thread_Descriptor *td = this->find_thread (id);
#endif /* ! VXWORKS */

    // Locate thread id.
    if (td != 0)
      {
        // @@ Currently, we have just one hook.  This should clearly
        // be generalized to support an arbitrary number of hooks.

        if (td->cleanup_info_.cleanup_hook_ != 0)
          {
            // Copy the hook so that we can call it after releasing
            // the guard.
            cleanup_info = td->cleanup_info_;
            td->cleanup_info_.cleanup_hook_ = 0;
          }

#if !defined (VXWORKS)
        // Threads created with THR_DAEMON shouldn't exist here, but
        // just to be safe, let's put it here.

        if (ACE_BIT_DISABLED (td->flags_, (THR_DETACHED | THR_DAEMON))
            || (ACE_BIT_ENABLED (td->flags_, THR_JOINABLE)))
          {
            // Mark thread as terminated.
            td->thr_state_ = ACE_THR_TERMINATED;
            this->terminated_thr_queue_.enqueue_tail (*td);
          }
#endif /* ! VXWORKS */

        // Remove thread descriptor from the table.
        this->remove_thr (td, 0);
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
ACE_Thread_Manager::wait (const ACE_Time_Value *timeout,
                          int abandon_detached_threads)
{
  ACE_TRACE ("ACE_Thread_Manager::wait");

#if defined (ACE_HAS_THREADS)
  {
    // Just hold onto the guard while waiting.
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

    if (ACE_Object_Manager::shutting_down () != 1)
      {
        // Program is not shutting down.  Perform a normal wait on threads.
        if (abandon_detached_threads != 0)
          {
            ACE_ASSERT (this->thr_to_be_removed_.is_empty ());
            for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor>
                   iter (this->thr_list_);
                 !iter.done ();
                 iter.advance ())
              if (ACE_BIT_ENABLED (iter.next ()->flags_, (THR_DETACHED | THR_DAEMON)) &&
                  ACE_BIT_DISABLED (iter.next ()->flags_, THR_JOINABLE))
                this->thr_to_be_removed_.enqueue_tail (iter.next ());

            if (! this->thr_to_be_removed_.is_empty ())
              {
                ACE_Thread_Descriptor *td;
                while (this->thr_to_be_removed_.dequeue_head (td) != -1)
                  this->remove_thr (td, 1);
              }
          }

        while (this->thr_list_.size () > 0)
          if (this->zero_cond_.wait (timeout) == -1)
            return -1;
      }
    else
        // Program is shutting down, no chance to wait on threads.
        // Therefore, we'll just remove threads from the list.
        this->remove_thr_all ();
    // Release the guard, giving other threads a chance to run.
  }

#if !defined (VXWORKS)
  // @@ VxWorks doesn't support thr_join (yet.)  We are working
  //on our implementation.   Chorus'es thr_join seems broken.
  ACE_Thread_Descriptor item;

#if defined (CHORUS)
  if (ACE_Object_Manager::shutting_down () != 1)
    {
#endif /* CHORUS */
      while (this->terminated_thr_queue_.dequeue_head (item) == 0)
        if (ACE_BIT_DISABLED (item.flags_, (THR_DETACHED | THR_DAEMON))
            || ACE_BIT_ENABLED (item.flags_, THR_JOINABLE))
          ACE_Thread::join (item.thr_handle_);
#if defined (CHORUS)
    }
#endif /* CHORUS */

#endif /* ! VXWORKS */
#else
  ACE_UNUSED_ARG (timeout);
  ACE_UNUSED_ARG (abandon_detached_threads);
#endif /* ACE_HAS_THREADS */

  return 0;
}

int
ACE_Thread_Manager::apply_task (ACE_Task_Base *task,
                                ACE_THR_MEMBER_FUNC func,
                                int arg)
{
  ACE_TRACE ("ACE_Thread_Manager::apply_task");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  ACE_ASSERT (this->thr_to_be_removed_.is_empty ());

  int result = 0;

  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    if (iter.next ()->task_ == task
        && (this->*func) (iter.next (), arg) == -1)
      result = -1;

  // Must remove threads after we have traversed the thr_list_ to
  // prevent clobber thr_list_'s integrity.

  if (! this->thr_to_be_removed_.is_empty ())
    {
      // Preserve errno!
      int error = errno;
      ACE_Thread_Descriptor *td;
      while (this->thr_to_be_removed_.dequeue_head (td) != -1)
        this->remove_thr (td, 1);
      errno = error;
    }

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
                    ACE_Thread_Descriptor [this->thr_list_.size ()],
                    -1);

    for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
         !iter.done ();
         iter.advance ())
      // If threads are created as THR_DETACHED or THR_DAEMON, we can't help much here.
      if (iter.next ()->task_ == task &&
          (ACE_BIT_DISABLED (iter.next ()->flags_,
                             (THR_DETACHED | THR_DAEMON)))
           || ACE_BIT_ENABLED (iter.next ()->flags_, THR_JOINABLE))
          copy_table[copy_count++] = *iter.next ();
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
                           ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::suspend_thr));
}

// Resume a task.
int
ACE_Thread_Manager::resume_task (ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_task");
  return this->apply_task (task,
                           ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::resume_thr));
}

// Kill a task.

int
ACE_Thread_Manager::kill_task (ACE_Task_Base *task, int /* signum */)
{
  ACE_TRACE ("ACE_Thread_Manager::kill_task");
  return this->apply_task (task,
                           ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::kill_thr));
}

// Cancel a task.
int
ACE_Thread_Manager::cancel_task (ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Thread_Manager::cancel_task");
  return this->apply_task (task,
                           ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::cancel_thr));
}

// Locate the index in the table associated with <task> from the
// beginning of the table up to an index.  Must be called with the
// lock held.

ACE_Thread_Descriptor *
ACE_Thread_Manager::find_task (ACE_Task_Base *task, int index)
{
  ACE_TRACE ("ACE_Thread_Manager::find_task");

  int i = 0;

  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (i >= index)
        break;

      if (task == iter.next ()->task_)
        return iter.next ();

      i++;
    }

  return 0;
}

// Returns the number of ACE_Task in a group.

int
ACE_Thread_Manager::num_tasks_in_group (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::num_tasks_in_group");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  int tasks_count = 0;
  size_t i = 0;

  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (iter.next ()->grp_id_ == grp_id
          && this->find_task (iter.next ()->task_, i) ==
          0)
        tasks_count++;

      i++;
    }
  return tasks_count;
}

// Returns the number of threads in an ACE_Task.

int
ACE_Thread_Manager::num_threads_in_task (ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Thread_Manager::num_threads_in_task");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  int threads_count = 0;

  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    if (iter.next ()->task_ == task)
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

  size_t i = 0;

  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (task_list_count >= n)
        break;

      if (iter.next ()->grp_id_ == grp_id
          && this->find_task (iter.next ()->task_, i) == 0)
        {
          task_list_iterator[task_list_count] = iter.next ()->task_;
          task_list_count++;
        }

      i++;
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

  size_t thread_count = 0;

  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (thread_count >= n)
        break;

      if (iter.next ()->task_ == task)
        {
          thread_list[thread_count] = iter.next ()->thr_id_;
          thread_count ++;
        }
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

  size_t hthread_count = 0;

  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (hthread_count >= n)
        break;

      if (iter.next ()->task_ == task)
        {
          hthread_list[hthread_count] = iter.next ()->thr_handle_;
          hthread_count ++;
        }
    }

  return 0;
}

int
ACE_Thread_Manager::set_grp (ACE_Task_Base *task, int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::set_grp");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    if (iter.next ()->task_ == task)
      iter.next ()->grp_id_ = grp_id;

  return 0;
}

int
ACE_Thread_Manager::get_grp (ACE_Task_Base *task, int &grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::get_grp");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_FIND (this->find_task (task), ptr);
  grp_id = ptr->grp_id_;
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

#if defined (ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS) || defined (ACE_HAS_TSS_EMULATION) || defined (ACE_WIN32)
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
#if !defined (VXWORKS)
  template class ACE_Unbounded_Queue<ACE_Thread_Descriptor>;
  template class ACE_Unbounded_Queue_Iterator<ACE_Thread_Descriptor>;
  template class ACE_Node<ACE_Thread_Descriptor>;
#endif /* ! VXWORKS */
  template class ACE_Unbounded_Queue<ACE_Thread_Descriptor*>;
  template class ACE_Unbounded_Queue_Iterator<ACE_Thread_Descriptor*>;
  template class ACE_Node<ACE_Thread_Descriptor*>;
  template class ACE_Double_Linked_List<ACE_Thread_Descriptor>;
  template class ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor>;
  // This doesn't necessarily belong here, but it's a convenient place for it.
  template class ACE_TSS_Singleton<ACE_Dynamic, ACE_Null_Mutex>;
# if (defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))
    // These don't necessarily belong here, but it's a convenient place for them.
    template class ACE_TSS<ACE_Dynamic>;
    template class ACE_TSS<ACE_Thread_Exit>;
# endif /* ACE_HAS_THREADS && (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION) */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if !defined (VXWORKS)
  #pragma instantiate ACE_Unbounded_Queue<ACE_Thread_Descriptor>
  #pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Thread_Descriptor>
  #pragma instantiate ACE_Node<ACE_Thread_Descriptor>
#endif /* ! VXWORKS */
  #pragma instantiate ACE_Unbounded_Queue<ACE_Thread_Descriptor*>
  #pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Thread_Descriptor*>
  #pragma instantiate ACE_Node<ACE_Thread_Descriptor*>
  #pragma instantiate ACE_Double_Linked_List<ACE_Thread_Descriptor>
  #pragma instantiate ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor>
  // This doesn't necessarily belong here, but it's a convenient place for it.
  #pragma instantiate ACE_TSS_Singleton<ACE_Dynamic, ACE_Null_Mutex>
# if (defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))
    // These don't necessarily belong here, but it's a convenient place for them.
    #pragma instantiate ACE_TSS<ACE_Dynamic>
    #pragma instantiate ACE_TSS<ACE_Thread_Exit>
# endif /* ACE_HAS_THREADS && (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION) */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
