// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    main_reactor.cpp             
//
// = DESCRIPTION
//      Implements an reactive timer queue.
//      This code exercises the Timer_Queue_Test_Driver class using
//      a reactor.
//
// = AUTHORS
//    Douglas Schmidt      <schmidt@cs.wustl.edu> &&
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>  
// 
// ============================================================================

#include "ace/Auto_Ptr.h"
#include "Driver.h"
#include "Reactor_Timer_Queue_Test.h"

typedef Timer_Queue_Test_Driver <ACE_Timer_Heap, Input_Handler>
	REACTOR_TIMER_QUEUE_TEST_DRIVER;

int
main (int, char *[])
{
  // Auto ptr ensures that the driver memory is released automatically.
  auto_ptr <REACTOR_TIMER_QUEUE_TEST_DRIVER> driver;
  REACTOR_TIMER_QUEUE_TEST_DRIVER *tqtd;
  
  ACE_NEW_RETURN (tqtd, Reactor_Timer_Queue_Test_Driver, -1);
  driver = tqtd;
  
  return driver->run_test ();
}
