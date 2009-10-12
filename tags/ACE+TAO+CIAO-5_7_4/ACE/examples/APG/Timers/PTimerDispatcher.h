/* -*- C++ -*- */
// $Id$

#if !defined(PTIMER_DISPATCHER_H)
#define PTIMER_DISPATCHER_H

#include "ace/Singleton.h"
#include "ace/Synch.h"  // needed for ACE_Event

#include "Upcall.h"
class PCB;

class PTimer_Dispatcher
{
public:
  void wait_for_event (void);

  long schedule (PCB *cb,
                 void *arg,
                 const ACE_Time_Value &abs_time,
                 const ACE_Time_Value &interval);

  int cancel (PCB *cb,
              int dont_call_handle_close = 1);

  int reset_interval (long timer_id,
                      const ACE_Time_Value &interval);

  void set (PTimerQueue *timer_queue);

private:
  PTimerQueue *timer_queue_;
  ACE_Event timer_;
};

typedef ACE_Singleton<PTimer_Dispatcher, ACE_Null_Mutex> PTimer;

#endif /*TIMER_DISPATCHER_H*/

