/* -*- C++ -*- */
#if !defined(TIMER_DISPATCHER_H)
#define TIMER_DISPATCHER_H

#include "ace/Event_Handler.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"  // needed for ACE_Event
#include "ace/Timer_Queue.h"

// Listing 1 code/ch20
class Timer_Dispatcher
{
public:
  Timer_Dispatcher () = default;
  Timer_Dispatcher (Timer_Dispatcher const &) = delete;
  Timer_Dispatcher (Timer_Dispatcher &&) = delete;
  Timer_Dispatcher &operator= (Timer_Dispatcher const &) = delete;
  Timer_Dispatcher &operator= (Timer_Dispatcher &&) = delete;

  void wait_for_event ();

  long schedule (ACE_Event_Handler *cb,
                 void *arg,
                 ACE_Time_Value const &abs_time,
                 ACE_Time_Value const &interval);

  int cancel (ACE_Event_Handler *cb,
              int dont_call_handle_close = 1);

  int reset_interval (long timer_id,
                      ACE_Time_Value const &interval);

  void set (ACE_Timer_Queue *timer_queue);

private:
  ACE_Timer_Queue *timer_queue_ {};
  ACE_Event timer_;
};

using Timer = ACE_Singleton<Timer_Dispatcher, ACE_Null_Mutex>;
// Listing 1

#endif /*TIMER_DISPATCHER_H*/
