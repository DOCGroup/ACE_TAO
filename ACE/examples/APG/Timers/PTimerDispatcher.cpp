// $Id$

#include "PTimerDispatcher.h"

void PTimer_Dispatcher::wait_for_event (void)
{
  ACE_TRACE ("PTimer_Dispatcher::wait_for_event");

  while (1)
    {
      ACE_Time_Value max_tv = timer_queue_->gettimeofday ();

      ACE_Time_Value *this_timeout =
        this->timer_queue_->calculate_timeout (&max_tv);

      if (*this_timeout == ACE_Time_Value::zero)
        this->timer_queue_->expire ();
      else
        {
          // Convert to absolute time.
          ACE_Time_Value next_timeout =
            timer_queue_->gettimeofday ();
          next_timeout += *this_timeout;
          if (this->timer_.wait (&next_timeout) == -1 )
            this->timer_queue_->expire ();
        }
    }
}

long
PTimer_Dispatcher::schedule (PCB *cb,
                             void *arg,
                             const ACE_Time_Value &abs_time,
                             const ACE_Time_Value &interval)
{
  ACE_TRACE ("PTimer_Dispatcher::schedule_timer");

  return this->timer_queue_->schedule
    (cb, arg, abs_time, interval);
}

int
PTimer_Dispatcher::cancel (PCB *cb,
                           int dont_call_handle_close)
{
  ACE_TRACE ("PTimer_Dispatcher::cancel");
  return timer_queue_->cancel (cb, dont_call_handle_close);
}

void PTimer_Dispatcher::set (PTimerQueue *timer_queue)
{
  ACE_TRACE ("PTimer_Dispatcher::set");

  timer_queue_ = timer_queue;
}

int
PTimer_Dispatcher::reset_interval (long timer_id,
                                   const ACE_Time_Value &interval)
{
  ACE_TRACE ("PTimer_Dispatcher::reset_interval");

  return timer_queue_->reset_interval (timer_id, interval);
}
ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, PTimer_Dispatcher, ACE_Null_Mutex);

