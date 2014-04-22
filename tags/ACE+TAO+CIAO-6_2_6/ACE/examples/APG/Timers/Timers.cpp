// $Id$

// Listing 1 code/ch20
#include "ace/Timer_Queue.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Wheel.h"
#include "ace/Timer_Hash.h"
#include "ace/Timer_List.h"

#include "CB.h"
#include "TimerDispatcher.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Timer_Queue *timer_queue;

#if defined(HEAP)

  ACE_NEW_RETURN (timer_queue, ACE_Timer_Heap, -1);
#elsif defined(HASH)

  ACE_NEW_RETURN (timer_queue, ACE_Timer_Hash, -1);
#elsif defined(WHEEL)

  ACE_NEW_RETURN (timer_queue, ACE_Timer_Wheel, -1);
#else

  ACE_NEW_RETURN (timer_queue, ACE_Timer_List, -1);
#endif

  // setup the timer queue
  Timer::instance ()->set (timer_queue);

  CB cb[10];
  long args[10];
  for (long i = 0; i < 10 ; i++)
    {
      ACE_Time_Value const timeout (i);
      long timerID =
        Timer::instance ()->schedule
          (&cb[i],
           &args[i],
           timer_queue->gettimeofday () + ACE_Time_Value(5),
           timeout);

      // Set the timerID state variable of the handler.
      cb[i].setID (timerID);

      // Implicitly send the handler it's timer id.
      args[i] = timerID;
    }

  // "run" the timer.
  Timer::instance ()->wait_for_event ();

  return 0;
}
// Listing 1
