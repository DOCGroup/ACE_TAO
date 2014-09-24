// $Id$

#include "TimerDispatcher.h"
// Listing 1 code/ch20
void Timer_Dispatcher::wait_for_event (void)
{
  ACE_TRACE ("Timer_Dispatcher::wait_for_event");

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
// Listing 1
// Listing 2 code/ch20
long
Timer_Dispatcher::schedule (ACE_Event_Handler *cb,
                            void *arg,
                            const ACE_Time_Value &abs_time,
                            const ACE_Time_Value &interval)
{
  ACE_TRACE ("Timer_Dispatcher::schedule_timer");

  return this->timer_queue_->schedule
    (cb, arg, abs_time, interval);
}
// Listing 2
// Listing 3 code/ch20
int
Timer_Dispatcher::cancel (ACE_Event_Handler *cb,
                          int dont_call_handle_close)
{
  ACE_TRACE ("Timer_Dispatcher::cancel");
  return timer_queue_->cancel (cb, dont_call_handle_close);
}
// Listing 3
// Listing 4 code/ch20
void Timer_Dispatcher::set (ACE_Timer_Queue *timer_queue)
{
  ACE_TRACE ("Timer_Dispatcher::set");

  timer_queue_ = timer_queue;
}
// Listing 4

int
Timer_Dispatcher::reset_interval (long timer_id,
                                  const ACE_Time_Value &interval)
{
  ACE_TRACE ("Timer_Dispatcher::reset_interval");

  return timer_queue_->reset_interval(timer_id, interval);
}
ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, Timer_Dispatcher, ACE_Null_Mutex);

