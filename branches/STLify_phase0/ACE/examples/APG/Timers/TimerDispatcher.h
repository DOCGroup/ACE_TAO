/* -*- C++ -*- */
// $Id$

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
  void wait_for_event (void);

  long schedule (ACE_Event_Handler *cb,
                 void *arg,
                 const ACE_Time_Value &abs_time,
                 const ACE_Time_Value &interval);

  int cancel (ACE_Event_Handler *cb,
              int dont_call_handle_close = 1);

  int reset_interval (long timer_id,
                      const ACE_Time_Value &interval);

  void set (ACE_Timer_Queue *timer_queue);

private:
  ACE_Timer_Queue *timer_queue_;
  ACE_Event timer_;
};

typedef ACE_Singleton<Timer_Dispatcher, ACE_Null_Mutex> Timer;
// Listing 1

#endif /*TIMER_DISPATCHER_H*/

