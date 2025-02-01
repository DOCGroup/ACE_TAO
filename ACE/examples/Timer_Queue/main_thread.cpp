
//=============================================================================
/**
 *  @file    main_thread.cpp
 *
 *    Implements an threaded timer queue.
 *    This code exercises the Timer_Queue_Test_Driver class using
 *    threads.
 *
 *  @author Douglas Schmidt      <d.schmidt@vanderbilt.edu> && Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================

#include "ace/OS_main.h"
#include "Driver.h"
#include "Thread_Timer_Queue_Test.h"
#include <memory>

typedef Timer_Queue_Test_Driver<Thread_Timer_Queue,
                                Input_Task,
                                Input_Task::ACTION>
  THREAD_TIMER_QUEUE_TEST_DRIVER;

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // unique ptr ensures that the driver memory is released
  // automatically.
  THREAD_TIMER_QUEUE_TEST_DRIVER *tqtd;
  ACE_NEW_RETURN (tqtd, Thread_Timer_Queue_Test_Driver, -1);

  std::unique_ptr <THREAD_TIMER_QUEUE_TEST_DRIVER> driver (tqtd);

  return driver->run_test ();
}

