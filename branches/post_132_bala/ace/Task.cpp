// $Id$

#include "ace/Task.h"
#include "ace/Module.h"

#if !defined (__ACE_INLINE__)
#include "ace/Task.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Task, "$Id$")

ACE_Task_Base::~ACE_Task_Base (void)
{
}

ACE_Task_Base::ACE_Task_Base (ACE_Thread_Manager *thr_man)
  : thr_count_ (0),
    thr_mgr_ (thr_man),
    flags_ (0),
    grp_id_ (-1)
{
}

// Wait for all threads running in a task to exit.

int
ACE_Task_Base::wait (void)
{
  ACE_TRACE ("ACE_Task_Base::wait");

  // If we don't have a thread manager, we probably were never
  // activated.
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

  return 0;
}

int
ACE_Task_Base::activate (long flags,
                         int n_threads,
                         int force_active,
                         long priority,
                         int grp_id,
                         ACE_Task_Base *task,
                         ACE_hthread_t thread_handles[],
                         void *stack[],
                         size_t stack_size[],
                         ACE_thread_t thread_ids[])
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
    {
      if (this->thr_count_ > 0 && grp_id != -1)
        // If we're joining an existing group of threads then make
        // sure to use its group id.
        grp_id = this->grp_id_;
      this->thr_count_ += n_threads;
    }

  // Use the ACE_Thread_Manager singleton if we're running as an
  // active object and the caller didn't supply us with a
  // Thread_Manager.
  if (this->thr_mgr_ == 0)
# if defined (ACE_THREAD_MANAGER_LACKS_STATICS)
    this->thr_mgr_ = ACE_THREAD_MANAGER_SINGLETON::instance ();
# else /* ! ACE_THREAD_MANAGER_LACKS_STATICS */
    this->thr_mgr_ = ACE_Thread_Manager::instance ();
# endif /* ACE_THREAD_MANAGER_LACKS_STATICS */

  int grp_spawned = -1;
  if (thread_ids == 0)
    // Thread Ids were not specified
    grp_spawned =
      this->thr_mgr_->spawn_n (n_threads,
                               &ACE_Task_Base::svc_run,
                               (void *) this,
                               flags,
                               priority,
                               grp_id,
                               task,
                               thread_handles,
                               stack,
                               stack_size);
  else
    // thread names were specified
    grp_spawned =
      this->thr_mgr_->spawn_n (thread_ids,
                               n_threads,
                               &ACE_Task_Base::svc_run,
                               (void *) this,
                               flags,
                               priority,
                               grp_id,
                               stack,
                               stack_size,
                               thread_handles,
                               task);
  if (grp_spawned == -1)
    {
      // If spawn_n fails, restore original thread count.
      this->thr_count_ -= n_threads;
      return -1;
    }

  if (this->grp_id_ == -1)
    this->grp_id_ = grp_spawned;

  return 0;

#else
  {
    // Keep the compiler from complaining.
    ACE_UNUSED_ARG (flags);
    ACE_UNUSED_ARG (n_threads);
    ACE_UNUSED_ARG (force_active);
    ACE_UNUSED_ARG (priority);
    ACE_UNUSED_ARG (grp_id);
    ACE_UNUSED_ARG (task);
    ACE_UNUSED_ARG (thread_handles);
    ACE_UNUSED_ARG (stack);
    ACE_UNUSED_ARG (stack_size);
    ACE_UNUSED_ARG (thread_ids);
    ACE_NOTSUP_RETURN (-1);
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


#if defined (ACE_HAS_SIG_C_FUNC)
extern "C" void
ACE_Task_Base_cleanup (void *object, void *)
{
  ACE_Task_Base::cleanup (object, 0);
}
#endif /* ACE_HAS_SIG_C_FUNC */

ACE_THR_FUNC_RETURN
ACE_Task_Base::svc_run (void *args)
{
  ACE_TRACE ("ACE_Task_Base::svc_run");

  ACE_Task_Base *t = (ACE_Task_Base *) args;

  // Register ourself with our <Thread_Manager>'s thread exit hook
  // mechanism so that our close() hook will be sure to get invoked
  // when this thread exits.

#if defined ACE_HAS_SIG_C_FUNC
  t->thr_mgr ()->at_exit (t, ACE_Task_Base_cleanup, 0);
#else
  t->thr_mgr ()->at_exit (t, ACE_Task_Base::cleanup, 0);
#endif /* ACE_HAS_SIG_C_FUNC */

  // Call the Task's svc() hook method.
  int svc_status = t->svc ();
  ACE_THR_FUNC_RETURN status;
#if (defined (__BORLANDC__) && (__BORLANDC__ < 0x570)) || defined (__MINGW32__) || (defined (_MSC_VER) && (_MSC_VER <= 1200))
  // Some compilers complain about reinterpret_cast from int to unsigned long...
  status = ACE_static_cast (ACE_THR_FUNC_RETURN, svc_status);
#else
  status = ACE_reinterpret_cast (ACE_THR_FUNC_RETURN, svc_status);
#endif /* (__BORLANDC__ < 0x570) || __MINGW32__ || _MSC_VER <= 1200 */

// If we changed this zero change the other if in OS.cpp Thread_Adapter::invoke
#if 1
  // Call the <Task->close> hook.
  ACE_Thread_Manager *thr_mgr_ptr = t->thr_mgr ();

  // This calls the Task->close () hook.
  t->cleanup (t, 0);

  // This prevents a second invocation of the cleanup code
  // (called later by <ACE_Thread_Manager::exit>.
  thr_mgr_ptr->at_exit (t, 0, 0);
#endif
  return status;
}

// Forward the call to close() so that existing applications don't
// break.

int
ACE_Task_Base::module_closed (void)
{
  return this->close (1);
}
