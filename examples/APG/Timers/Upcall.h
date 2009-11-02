/* -*- C++ -*- */
// $Id$

#if !defined(UPCALL_H)
#define UPCALL_H

#include "ace/Timer_Queue_T.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Synch.h"

#include "PCB.h"

// Listing 1 code/ch20
class UpcallHandler;

typedef ACE_Timer_Queue_T<PCB*, UpcallHandler, ACE_Null_Mutex>
  PTimerQueue;

// Create a special heap-based timer queue that allows you to
// control exactly how timer evetns are handled.
typedef ACE_Timer_Heap_T<PCB*, UpcallHandler, ACE_Null_Mutex>
  PTimerHeap;
// Listing 1

class UpcallHandler
{
public:
  // The signature of this method changed at ACE 5.4. The 'recurring_timer'
  // parameter was added.
  int timeout (PTimerQueue &timer_queue,
               PCB *handler,
               const void *arg,
               int recurring_timer,
               const ACE_Time_Value &cur_time);

#if 0
  // This method was removed at ACE 5.4. Replaced by cancel_type() and
  // cancel_timer().
  // This method is called when the timer is canceled.
  int cancellation (PTimerQueue &timer_queue,
                    PCB *handler);
#endif

  // This method is called when the timer queue is destroyed and
  // the timer is still contained in it.
  int deletion (PTimerQueue &timer_queue,
                PCB *handler,
                const void *arg);

  // The following methods don't appear before ACE 5.4, so aren't
  // referenced in APG (it's based on ACE 5.3).

  // This method is called when a timer is registered.
  int registration (PTimerQueue &timer_queue,
                    PCB *handler,
                    const void *arg);

  // This method is called before the timer expires.
  int preinvoke (PTimerQueue &timer_queue,
                 PCB *handler,
                 const void *arg,
                 int recurring_timer,
                 const ACE_Time_Value &cur_time,
                 const void *&upcall_act);

  // This method is called after the timer expires.
  int postinvoke (PTimerQueue &timer_queue,
                  PCB *handler,
                  const void *arg,
                  int recurring_timer,
                  const ACE_Time_Value &cur_time,
                  const void *upcall_act);

  // This method is called when a handler is cancelled
  int cancel_type (PTimerQueue &timer_queue,
                   PCB *handler,
                   int dont_call,
                   int &requires_reference_counting);

  // This method is called when a timer is cancelled
  int cancel_timer (PTimerQueue &timer_queue,
                    PCB *handler,
                    int dont_call,
                    int requires_reference_counting);
};

#endif /*UPCALL_H*/
