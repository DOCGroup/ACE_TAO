// Task.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Task.h"
#include "ace/Module.h"
#include "ace/Service_Config.h"

#if !defined (__ACE_INLINE__)
#include "ace/Task.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
#if (defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE))
  // For template specializations at end of this file.
  #include "ace/Dynamic.h"
#endif /* ACE_HAS_THREADS && ACE_HAS_THREAD_SPECIFIC_STORAGE */
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#if defined (ACE_MT_SAFE)
// Lock the creation of the Singleton.
ACE_Thread_Mutex ACE_Task_Exit::ace_task_lock_;
#endif /* defined (ACE_MT_SAFE) */

// NOTE:  this preprocessor directive should match the one in
// ACE_Task_Base::svc_run () below.  This prevents the two statics
// from being defined.
#if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) && ! defined (ACE_HAS_PTHREAD_SIGMASK)
ACE_Task_Exit *
ACE_Task_Exit::instance (void)
{
  ACE_TRACE ("ACE_Task_Exit::instance");

  // Determines if we were dynamically allocated.  
  static ACE_TSS_TYPE (ACE_Task_Exit) *instance_;

  // Implement the Double Check pattern.

  if (instance_ == 0)
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ace_task_lock_, 0));

      if (instance_ == 0)
	ACE_NEW_RETURN (instance_, ACE_TSS_TYPE (ACE_Task_Exit), 0);
    }

  return ACE_TSS_GET (instance_, ACE_Task_Exit);

}
#endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE && ! ACE_HAS_PTHREAD_SIGMASK */


// Grab hold of the Task * so that we can close() it in the
// destructor.

ACE_Task_Exit::ACE_Task_Exit (void)
  : t_ (0), 
    status_ ((void *) -1)
{
  ACE_TRACE ("ACE_Task_Exit::ACE_Task_Exit");
}

// Returns the pointer to the ACE_Task.

ACE_Task_Base *
ACE_Task_Exit::get_task (void)
{
  ACE_TRACE ("ACE_Task_Exit::get_task");

  return this->t_;
}

// Set the this pointer...

void
ACE_Task_Exit::set_task (ACE_Task_Base *t)
{
  ACE_TRACE ("ACE_Task_Exit::set_task");
  this->t_ = t;

  if (t != 0)
    this->tc_.insert (t->thr_mgr ());
}

// Set the thread exit status value.

void *
ACE_Task_Exit::status (void *s)
{
  ACE_TRACE ("ACE_Task_Exit::status");
  return this->status_ = s;
}

void *
ACE_Task_Exit::status (void)
{
  ACE_TRACE ("ACE_Task_Exit::status");
  return this->status_;
}

// When this object is destroyed the Task is automatically closed
// down!

ACE_Task_Exit::~ACE_Task_Exit (void)
{
  ACE_TRACE ("ACE_Task_Exit::~ACE_Task_Exit");

  if (this->t_ != 0)
    {
      // The thread count must be decremented first in case the
      // close() hook does something crazy like "delete this".
      this->t_->thr_count_dec ();
      this->t_->close (u_long (this->status_));
    }
}

ACE_ALLOC_HOOK_DEFINE(ACE_Task)

ACE_Task_Base::ACE_Task_Base (ACE_Thread_Manager *thr_man)
  : thr_count_ (0),
    thr_mgr_ (thr_man),
    flags_ (0),
    grp_id_ (0)
{
}

// Wait for all threads running in a task to exit.
int 
ACE_Task_Base::wait (void)
{
  ACE_TRACE ("ACE_Task_Base::wait");

  // If we don't have a thread manager, we probably were never activated
  if (this->thr_mgr () != 0)
    return this->thr_mgr ()->wait_task (this);
  else
    return 0;
}

// Suspend a task.
int 
ACE_Task_Base::suspend (void)
{
  ACE_TRACE ("ACE_Task_Base::suspend");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  if (this->thr_count_ > 0)
    return this->thr_mgr_->suspend_task (this);
  else
    return 0;
}

// Resume a suspended task.
int 
ACE_Task_Base::resume (void)
{
  ACE_TRACE ("ACE_Task_Base::resume");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  if (this->thr_count_ > 0)
    return this->thr_mgr_->resume_task (this);
  else
    return 0;
}

int
ACE_Task_Base::activate (long flags, 
			 int n_threads, 
			 int force_active,
			 long priority,
			 int grp_id,
			 ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Task_Base::activate");

#if defined (ACE_MT_SAFE)
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);
  
  // If the task passed in is zero, we will use <this>
  if (task == 0)
    task = this;

  if (this->thr_count_ > 0 && force_active == 0)
    return 1; // Already active.
  else
    this->thr_count_ += n_threads;

  // Use the ACE_Thread_Manager singleton if we're running as an
  // active object and the caller didn't supply us with a
  // Thread_Manager.
  if (this->thr_mgr_ == 0)   
    this->thr_mgr_ = ACE_Service_Config::thr_mgr ();

  this->grp_id_ = this->thr_mgr_->spawn_n (n_threads, 
					   ACE_THR_FUNC (&ACE_Task_Base::svc_run),
					   (void *) this, 
					   flags,
					   priority,
					   grp_id,
					   task);
  if (this->grp_id_ == -1)
    return -1;
  else
    return 0;
#else
  {
    // Keep the compiler from complaining.
    ACE_UNUSED_ARG (n_threads);
    ACE_UNUSED_ARG (force_active);
    ACE_UNUSED_ARG (priority);
    ACE_UNUSED_ARG (grp_id);
    ACE_UNUSED_ARG (task);
    ACE_UNUSED_ARG (flags);
    errno = EINVAL;
    return -1;
  }
#endif /* ACE_MT_SAFE */
}

// Note that this routine often does not return since the thread that
// is executing it will do an ACE_Thread::exit() first!

// The ACE_Task_Exit - ACE_Task_Base::svc_run () interaction works
// like this, with ACE_HAS_THREAD_SPECIFIC_STORAGE:
// o Every thread in an ACE task is run via
//   ACE_Task_Base::svc_run ().
// o ACE_Task_Base::svc_run () retrieves the singleton
//   ACE_Task_Exit instance from ACE_Task_Exit::instance ().
//   The singleton gets created in thread specific storage
//   in the first call to that function for an ACE_Task.
//   The key point is that the instance is in thread specific
//   storage.
// o The ACE_Task is destroyed, usually by the application
//   following a call to Thread_Manager::wait (), which waits for
//   all of the task's threads to finish.  Alternatively, all of
//   the threads can exit on their own.
// o If you follow this so far, now it gets really fun . . .
//   When the thread specific storage (for the ACE_Task that
//   is being destroyed) is cleaned up, the threads package is
//   supposed to destroy any objects that are in thread specific
//   storage.  It has a list of 'em, and just walks down the
//   list and destroys each one.
// o That's where the ACE_Task_Exit destructor gets called.

void *
ACE_Task_Base::svc_run (void *args)
{
  ACE_TRACE ("ACE_Task_Base::svc_run");

  ACE_Task_Base *t = (ACE_Task_Base *) args;

// NOTE:  this preprocessor directive should match the one in
// above ACE_Task_Exit::instance ().
// With the Xavier Pthreads package, the exit_hook in TSS causes
// a seg fault.  So, this works around that by creating exit_hook
// on the stack.
#if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) && ! defined (ACE_HAS_PTHREAD_SIGMASK)
  // Obtain our thread-specific exit hook and make sure that it knows
  // how to clean us up!  Note that we never use this pointer directly
  // (it's stored in thread-specific storage), so it's ok to
  // dereference it here and only store it as a reference.
  ACE_Task_Exit &exit_hook = *ACE_Task_Exit::instance ();
#else
  // Without TSS, create an ACE_Task_Exit instance.  When this
  // function returns, its destructor will be called because the
  // object goes out of scope.  The drawback with this appraoch is
  // that the destructor _won't_ get called if thr_exit () is called.
  // So, threads shouldn't exit that way.  Instead, they should
  // return from svc ().
  ACE_Task_Exit exit_hook;
#endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE && ! ACE_HAS_PTHREAD_SIGMASK */

  exit_hook.set_task (t);

  // Call the Task's svc() method.
  void *status = (void *) t->svc ();

  return exit_hook.status (status);
  /* NOTREACHED */
}

// Forward the call to close() so that existing applications don't
// break.

int 
ACE_Task_Base::module_closed (void)
{
  return this->close (1);
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
#if (defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE))
  template class ACE_TSS<ACE_Task_Exit>;
  // This doesn't necessarily belong here, but it's a convenient place for it.
  template class ACE_TSS<ACE_Dynamic>;
#endif /* ACE_HAS_THREADS && ACE_HAS_THREAD_SPECIFIC_STORAGE */
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
