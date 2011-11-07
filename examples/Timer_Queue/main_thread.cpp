
//=============================================================================
/**
 *  @file    main_thread.cpp
 *
 *  $Id$
 *
 *    Implements an threaded timer queue.
 *    This code exercises the Timer_Queue_Test_Driver class using
 *    threads.
 *
 *
 *  @author Douglas Schmidt      <schmidt@cs.wustl.edu> && Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#include "ace/OS_main.h"
#include "ace/Auto_Ptr.h"
#include "Driver.h"
#include "Thread_Timer_Queue_Test.h"

typedef Timer_Queue_Test_Driver<Thread_Timer_Queue,
                                Input_Task,
                                Input_Task::ACTION>
  THREAD_TIMER_QUEUE_TEST_DRIVER;

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Auto ptr ensures that the driver memory is released
  // automatically.
  THREAD_TIMER_QUEUE_TEST_DRIVER *tqtd;
  ACE_NEW_RETURN (tqtd, Thread_Timer_Queue_Test_Driver, -1);

  auto_ptr <THREAD_TIMER_QUEUE_TEST_DRIVER> driver (tqtd);

  return driver->run_test ();
}

