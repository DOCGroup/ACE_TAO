/* -*- C++ -*- */
// $Id$

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
