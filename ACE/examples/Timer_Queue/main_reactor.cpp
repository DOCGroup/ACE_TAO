
//=============================================================================
/**
 *  @file    main_reactor.cpp
 *
 *    Implements an reactive timer queue.
 *    This code exercises the Timer_Queue_Test_Driver class using
 *    a reactor.
 *
 *  @author Douglas Schmidt <d.schmidt@vanderbilt.edu> && Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================

#include "ace/OS_main.h"
#include "Reactor_Timer_Queue_Test.h"
#include "Driver.h"
#include <memory>
#include <new>

using REACTOR_TIMER_QUEUE_TEST_DRIVER = Timer_Queue_Test_Driver <ACE_Timer_Heap,
                                                                 Input_Handler,
                                                                 Input_Handler::ACTION>;

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  std::unique_ptr<REACTOR_TIMER_QUEUE_TEST_DRIVER> driver (
    new (std::nothrow) Reactor_Timer_Queue_Test_Driver);
  if (!driver)
    return -1;

  return driver->run_test ();
}
