// $Id$

#include "ace/Log_Msg.h"
#include "Upcall.h"
#include "PTimerDispatcher.h"

// Listing 2 code/ch20
int 
UpcallHandler::timeout (PTimerQueue &timer_queue,
                        PCB *handler,
                        const void *arg,
                        const ACE_Time_Value &cur_time)
{
  ACE_TRACE (ACE_TEXT ("UpcallHandler::timeout"));

  return (*handler).handleEvent (arg);
}

int
UpcallHandler::cancellation (PTimerQueue &timer_queue,
                             PCB *handler)
{
  ACE_TRACE (ACE_TEXT ("UpcallHandler::cancellation"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Handler %d has been cancelled\n"),
              handler->getID ()));

  return handler->handleCancel ();
}

// This method is called when the timer is canceled
int
UpcallHandler::deletion (PTimerQueue &timer_queue,
                         PCB  *handler,
                         const void *arg)
{
  ACE_TRACE (ACE_TEXT ("UpcallHandler::deletion"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Handler %d has been deleted\n"),
              handler->getID ()));

  return handler->handleClose ();
}
// Listing 2


// Listing 3 code/ch20
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  PCB cb1, cb2;
  cb1.setID (1);
  cb2.setID (2);
  int arg1 = 1, arg2 = 2;

  PTimerQueue *timerQueue;

  ACE_NEW_RETURN (timerQueue, PTimerHeap (), -1);

  PTimer::instance ()->set (timerQueue);

  ACE_Time_Value tv = ACE_OS::gettimeofday ();
  tv += 20L;

  // Schedule two different timers to go off.
  PTimer::instance ()->schedule (&cb1, &arg1, tv, 1);
  PTimer::instance ()->schedule (&cb2, &arg2, tv, 2);

  // Run the timer event loop forever.
  PTimer::instance ()->wait_for_event ();

  return 0;
}
// Listing 3
