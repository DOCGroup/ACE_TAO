/* -*- C++ -*- */

// $Id$

template<class TQ> ACE_INLINE
ACE_Thread_Timer_Queue_Adapter<TQ>::ACE_Thread_Timer_Queue_Adapter (void)
  : ACE_Task_Base (ACE_Thread_Manager::instance ()),
    condition_ (lock_)
{
  // Assume that we start in active mode.
  this->active_ = 1;
}

template<class TQ> ACE_INLINE ACE_SYNCH_MUTEX &
ACE_Thread_Timer_Queue_Adapter<TQ>::mutex (void)
{
  return this->lock_;
}

template<class TQ> ACE_INLINE TQ &
ACE_Thread_Timer_Queue_Adapter<TQ>::timer_queue (void)
{
  return timer_queue_;
}
