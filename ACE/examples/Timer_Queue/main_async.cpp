
//=============================================================================
/**
 *  @file    main_async.cpp
 *
 *    Implements an asynchronous timer queue.
 *    This code exercises the Timer_Queue_Test_Driver class using
 *    signals as an asynchronous mechanism to dispatch events.
 *
 *  @author Douglas Schmidt      <d.schmidt@vanderbilt.edu> && Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================

#include "ace/OS_main.h"
#include "Driver.h"
#include "Async_Timer_Queue_Test.h"
#include <memory>
#include <new>

using ASYNC_TIMER_QUEUE_TEST_DRIVER = Timer_Queue_Test_Driver<Async_Timer_Queue *,
                                                              Async_Timer_Queue,
                                                              Async_Timer_Queue::ACTION>;

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  std::unique_ptr<ASYNC_TIMER_QUEUE_TEST_DRIVER> driver (
    new (std::nothrow) Async_Timer_Queue_Test_Driver);
  if (!driver)
    return -1;

  return driver->run_test ();
}
