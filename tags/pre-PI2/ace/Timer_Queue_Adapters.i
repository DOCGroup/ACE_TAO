/* -*- C++ -*- */
// $Id$

template<class TQ> ACE_INLINE TQ &
ACE_Thread_Timer_Queue_Adapter<TQ>::timer_queue (void)
{
  return this->timer_queue_;
}

template<class TQ> ACE_INLINE ACE_thread_t
ACE_Thread_Timer_Queue_Adapter<TQ>::thr_id (void)
{
  return this->thr_id_;
}

template<class TQ> ACE_INLINE int
ACE_Thread_Timer_Queue_Adapter<TQ>::activate (long flags,
                                              int n_threads,
                                              int force_active,
                                              long priority,
                                              int grp_id,
                                              ACE_Task_Base *task,
                                              ACE_hthread_t thread_handles[],
                                              void *stack[],
                                              size_t stack_size[],
                                              ACE_thread_t thread_names[])
{
  // Macros to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (n_threads);
  ACE_UNUSED_ARG (force_active);
  ACE_UNUSED_ARG (thread_handles);

  // Make sure that we only allow a single thread to be spawned for
  // our adapter.  Otherwise, too many weird things can happen.
  return ACE_Task_Base::activate (flags, 1, 0, priority, grp_id, task, 0,
                                  stack, stack_size, thread_names);
}
