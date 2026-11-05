
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
#include <new>

using THREAD_TIMER_QUEUE_TEST_DRIVER = Timer_Queue_Test_Driver<Thread_Timer_Queue,
                                                                Input_Task,
                                                                Input_Task::ACTION>;

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  std::unique_ptr<THREAD_TIMER_QUEUE_TEST_DRIVER> driver (
    new (std::nothrow) Thread_Timer_Queue_Test_Driver);
  if (!driver)
    return -1;

  return driver->run_test ();
}
