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

// The following #pragma is needed to disable a warning that occurs
// in MSVC 6 due to the overly long debugging symbols generated for
// the std::auto_ptr<Timer_Queue_Test_Driver<...> > template
// instance used by some of the methods in this file.
#ifdef _MSC_VER
#  pragma warning(disable: 4786)  /* identifier was truncated to '255'
                                     characters in the browser
                                     information */
#endif  /* _MSC_VER */

#include "ace/Auto_Ptr.h"
#include "Driver.h"
#include "Thread_Timer_Queue_Test.h"

ACE_RCSID (Timer_Queue,
           main_thread,
           "$Id$")

typedef Timer_Queue_Test_Driver<Thread_Timer_Queue,
                                Input_Task,
				Input_Task::ACTION>
	THREAD_TIMER_QUEUE_TEST_DRIVER;

int
ACE_TMAIN (int, ACE_TCHAR *[])
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
