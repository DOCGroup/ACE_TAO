/* -*- C++ -*- */
// $Id$

template<class TQ> ACE_INLINE TQ &
ACE_Thread_Timer_Queue_Adapter<TQ>::timer_queue (void)
{
  return *(this->timer_queue_);
}

template<class TQ> ACE_INLINE TQ *
ACE_Thread_Timer_Queue_Adapter<TQ>::timer_queue (void) const
{
  return this->timer_queue_;
}

template<class TQ> ACE_INLINE int
ACE_Thread_Timer_Queue_Adapter<TQ>::timer_queue (TQ *tq)
{
  if (this->delete_timer_queue_ != 0)
    delete this->timer_queue_;
  this->timer_queue_ = tq;
  this->delete_timer_queue_ = 0;
  return 0;
}

template<class TQ> ACE_INLINE ACE_thread_t
ACE_Thread_Timer_Queue_Adapter<TQ>::thr_id (void) const
{
  return this->thr_id_;
}

template<class TQ> ACE_INLINE int
ACE_Thread_Timer_Queue_Adapter<TQ>::activate (long flags,
                                              int ,
                                              int ,
                                              long priority,
                                              int grp_id,
                                              ACE_Task_Base *task,
                                              ACE_hthread_t thread_handles[],
                                              void *stack[],
                                              size_t stack_size[],
                                              ACE_thread_t thread_names[],
                                              bool )
{
  // Macros to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (thread_handles);

  // Make sure that we only allow a single thread to be spawned for
  // our adapter.  Otherwise, too many weird things can happen.
  return ACE_Task_Base::activate (flags, 1, 0, priority, grp_id, task, 0,
                                  stack, stack_size, thread_names);
}
