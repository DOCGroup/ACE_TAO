
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

typedef Timer_Queue_Test_Driver <ACE_Timer_Heap,
                                 Input_Handler,
                                 Input_Handler::ACTION>
        REACTOR_TIMER_QUEUE_TEST_DRIVER;

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  REACTOR_TIMER_QUEUE_TEST_DRIVER *tqtd;
  ACE_NEW_RETURN (tqtd, Reactor_Timer_Queue_Test_Driver, -1);
  // unique ptr ensures that the driver memory is released
  // automatically.
  std::unique_ptr <REACTOR_TIMER_QUEUE_TEST_DRIVER> driver (tqtd);

  return driver->run_test ();
}

