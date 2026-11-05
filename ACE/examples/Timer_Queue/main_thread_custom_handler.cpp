//=============================================================================
/**
 *  @file    main_thread_custom_handler.cpp
 *
 *    Implements a threaded timer queue.
 *    This code exercises the Timer_Queue_Test_Driver class using
 *    threads.
 *    It also uses custom event handlers for timer events.
 *
 *  @author Douglas Schmidt <d.schmidt@vanderbilt.edu>, Sergio Flores-Gaitan
 *          <sergio@cs.wustl.edu>, and Alon Diamant <diamant.alon@gmail.com>
 */
//=============================================================================


#include "ace/OS_main.h"
#include "Driver.h"
#include "Thread_Timer_Queue_Custom_Handler_Test.h"
#include <memory>
#include <new>

using THREAD_TIMER_QUEUE_TEST_DRIVER =
  Timer_Queue_Test_Driver<Thread_Timer_Queue,
                          Custom_Handler_Input_Task,
                          Custom_Handler_Input_Task::ACTION>;

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  std::unique_ptr<THREAD_TIMER_QUEUE_TEST_DRIVER> driver (
    new (std::nothrow) Thread_Timer_Queue_Custom_Handler_Test);
  if (!driver)
    return -1;

  return driver->run_test ();
}
