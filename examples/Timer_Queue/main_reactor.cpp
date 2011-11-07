
//=============================================================================
/**
 *  @file    main_reactor.cpp
 *
 *  $Id$
 *
 *    Implements an reactive timer queue.
 *    This code exercises the Timer_Queue_Test_Driver class using
 *    a reactor.
 *
 *
 *  @author Douglas Schmidt      <schmidt@cs.wustl.edu> && Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#include "ace/OS_main.h"
#include "ace/Auto_Ptr.h"
#include "Reactor_Timer_Queue_Test.h"
#include "Driver.h"

typedef Timer_Queue_Test_Driver <ACE_Timer_Heap,
                                 Input_Handler,
                                 Input_Handler::ACTION>
        REACTOR_TIMER_QUEUE_TEST_DRIVER;

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  REACTOR_TIMER_QUEUE_TEST_DRIVER *tqtd;
  ACE_NEW_RETURN (tqtd, Reactor_Timer_Queue_Test_Driver, -1);
  // Auto ptr ensures that the driver memory is released
  // automatically.
  auto_ptr <REACTOR_TIMER_QUEUE_TEST_DRIVER> driver (tqtd);

  return driver->run_test ();
}

