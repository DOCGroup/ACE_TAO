// Task.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Task.h"
#include "ace/Module.h"
#include "ace/Service_Config.h"

#if !defined (__ACE_INLINE__)
#include "ace/Task.i"
#endif /* __ACE_INLINE__ */


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
			 ACE_Task_Base *task,
			 ACE_hthread_t thread_handles[])
{
  ACE_TRACE ("ACE_Task_Base::activate");

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
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
    this->thr_mgr_ = ACE_Thread_Manager::instance ();

  this->grp_id_ = this->thr_mgr_->spawn_n (n_threads,
					   ACE_THR_FUNC (&ACE_Task_Base::svc_run),
					   (void *) this,
					   flags,
					   priority,
					   grp_id,
					   task,
					   thread_handles);
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

void
ACE_Task_Base::cleanup (void *object, void *)
{
  ACE_Task_Base *t = (ACE_Task_Base *) object;
  
  // The thread count must be decremented first in case the <close>
  // hook does something crazy like "delete this".
  t->thr_count_dec ();
  // @@ Is it possible to pass in the exit status somehow?
  t->close ();
}

void *
ACE_Task_Base::svc_run (void *args)
{
  ACE_TRACE ("ACE_Task_Base::svc_run");

  ACE_Task_Base *t = (ACE_Task_Base *) args;

  // Register ourself with our <Thread_Manager>'s thread exit hook
  // mechanism so that our close() hook will be sure to get invoked
  // when this thread exits.
  t->thr_mgr ()->at_exit (t, ACE_Task_Base::cleanup, 0);

  // Call the Task's svc() hook method.
  return (void *) t->svc ();

  /* NOTREACHED */
}

// Forward the call to close() so that existing applications don't
// break.

int
ACE_Task_Base::module_closed (void)
{
  return this->close (1);
}

