// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    main_thread.cpp
//
// = DESCRIPTION
//      Implements an threaded timer queue.
//      This code exercises the Timer_Queue_Test_Driver class using
//      threads.
//
// = AUTHORS
//    Douglas Schmidt      <schmidt@cs.wustl.edu> &&
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#include "ace/Auto_Ptr.h"
#include "Driver.h"
#include "Thread_Timer_Queue_Test.h"

typedef Timer_Queue_Test_Driver<Thread_Timer_Queue,
                                Input_Task,
				Input_Task::ACTION>
	THREAD_TIMER_QUEUE_TEST_DRIVER;

int
main (int, char *[])
{
  // Auto ptr ensures that the driver memory is released
  // automatically.
  THREAD_TIMER_QUEUE_TEST_DRIVER *tqtd;
  ACE_NEW_RETURN (tqtd, Thread_Timer_Queue_Test_Driver, -1);

  auto_ptr <THREAD_TIMER_QUEUE_TEST_DRIVER> driver (tqtd);

  return driver->run_test ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr <THREAD_TIMER_QUEUE_TEST_DRIVER>;
template class ACE_Auto_Basic_Ptr <THREAD_TIMER_QUEUE_TEST_DRIVER>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr <THREAD_TIMER_QUEUE_TEST_DRIVER>
#pragma instantiate ACE_Auto_Basic_Ptr <THREAD_TIMER_QUEUE_TEST_DRIVER>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
