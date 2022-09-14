
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

typedef Timer_Queue_Test_Driver<Async_Timer_Queue *,
                                Async_Timer_Queue,
                                Async_Timer_Queue::ACTION>
        ASYNC_TIMER_QUEUE_TEST_DRIVER;

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ASYNC_TIMER_QUEUE_TEST_DRIVER *tqtd;
  ACE_NEW_RETURN (tqtd, Async_Timer_Queue_Test_Driver, -1);
  // unique ptr ensures that the driver memory is released
  // automatically.
  std::unique_ptr <ASYNC_TIMER_QUEUE_TEST_DRIVER> driver (tqtd);

  return driver->run_test ();
}

