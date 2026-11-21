/* -*- C++ -*- */
#if !defined(PTIMER_DISPATCHER_H)
#define PTIMER_DISPATCHER_H

#include "ace/Singleton.h"
#include "ace/Synch.h"  // needed for ACE_Event

#include "Upcall.h"
class PCB;

class PTimer_Dispatcher
{
public:
  PTimer_Dispatcher () = default;
  PTimer_Dispatcher (PTimer_Dispatcher const &) = delete;
  PTimer_Dispatcher (PTimer_Dispatcher &&) = delete;
  PTimer_Dispatcher &operator= (PTimer_Dispatcher const &) = delete;
  PTimer_Dispatcher &operator= (PTimer_Dispatcher &&) = delete;

  void wait_for_event ();

  long schedule (PCB *cb,
                 void *arg,
                 ACE_Time_Value const &abs_time,
                 ACE_Time_Value const &interval);

  int cancel (PCB *cb,
              int dont_call_handle_close = 1);

  int reset_interval (long timer_id,
                      ACE_Time_Value const &interval);

  void set (PTimerQueue *timer_queue);

private:
  PTimerQueue *timer_queue_ {};
  ACE_Event timer_;
};

using PTimer = ACE_Singleton<PTimer_Dispatcher, ACE_Null_Mutex>;

#endif /*TIMER_DISPATCHER_H*/
