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
  int timeout (PTimerQueue &timer_queue,
               PCB *handler,
               const void *arg,
               const ACE_Time_Value &cur_time);

  /// This method is called when a timer is registered.
  int registration (PTimerQueue &timer_queue,
                    ACE_Event_Handler *handler,
                    const void *arg);

  // This method is called when the timer is canceled.
  int cancellation (PTimerQueue &timer_queue,
                    PCB *handler);

  // This method is called when the timer queue is destroyed and
  // the timer is still contained in it.
  int deletion (PTimerQueue &timer_queue,
                PCB *handler,
                const void *arg);
};

#endif /*UPCALL_H*/
