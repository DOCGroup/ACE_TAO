// $Id: test_timeout.cpp

// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_timeout.cpp
//
// = DESCRIPTION
//
//    This example application shows how to write event loops that
//    handle events for some fixed amount of time. Note that any
//    thread in the Proactor thread pool can call back the handler. On
//    POSIX4 systems, this test works only with POSIX_SIG_Proactor,
//    which can work with multiple threads.
//
// = AUTHOR
//    Irfan Pyarali and Alexander Babu Arulanthu
// 
// ============================================================================

#include "ace/Proactor.h"
#include "ace/Synch.h"
#include "ace/Task.h"

ACE_RCSID(Proactor, test_timeout, "$Id$")

#if ((defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || \
     (defined (ACE_HAS_AIO_CALLS)) && !defined (ACE_POSIX_AIOCB_PROACTOR))
  // This only works on Win32 platforms and on Unix platforms supporting
  // POSIX aio calls.

class Timeout_Handler : public ACE_Handler
{
  // = TITLE
  //     Generic timeout handler.
public:
  Timeout_Handler (void)
    : start_time_ (ACE_OS::gettimeofday ())
    {
    }

  virtual void handle_time_out (const ACE_Time_Value &tv,
				const void *arg)
    {
      // Print out when timeouts occur.
      ACE_DEBUG ((LM_DEBUG, "(%t) %d timeout occurred for %s @ %d.\n", 
		  ++count_,
		  (char *) arg,
		  (tv - this->start_time_).sec ()));
      
      // Sleep for a while
      ACE_OS::sleep (4);
    }
  
private:
  ACE_Atomic_Op <ACE_Thread_Mutex, int> count_;
  // Number of the timer event.
  
  ACE_Time_Value start_time_;
  // Starting time of the test.
};

class Worker : public ACE_Task <ACE_NULL_SYNCH>
{
public:
  int svc (void) 
    {
      // Handle events for 13 seconds.
      ACE_Time_Value run_time (13);
      
      ACE_DEBUG ((LM_DEBUG, "(%t):Starting svc routine\n"));
      
      if (ACE_Proactor::run_event_loop(run_time) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%t):%p.\n", "Worker::svc"), -1);
      
      ACE_DEBUG ((LM_DEBUG, "(%t) work complete\n"));
      
      return 0;
    }
};

int
main (int, char *[])
{
  Timeout_Handler handler;

  // Register a 2 second timer.
  ACE_Time_Value foo_tv (2);
  if (ACE_Proactor::instance ()->schedule_timer (handler,
                                                 (void *) "Foo",
                                                 ACE_Time_Value::zero,
                                                 foo_tv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

  // Register a 3 second timer.
  ACE_Time_Value bar_tv (3);
  if (ACE_Proactor::instance ()->schedule_timer (handler,
                                                 (void *) "Bar",
                                                 ACE_Time_Value::zero,
                                                 bar_tv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

  Worker worker;
  
  if (worker.activate (THR_NEW_LWP, 10) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n", "main"), -1);

  ACE_Thread_Manager::instance ()->wait ();
  
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<ACE_Thread_Mutex, int>;
template class ACE_Atomic_Op_Ex<ACE_Thread_Mutex, int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, int>
#pragma instantiate ACE_Atomic_Op_Ex<ACE_Thread_Mutex, int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else /* ACE_WIN32 && !ACE_HAS_WINCE || ACE_HAS_AIO_CALLS && !ACE_POSIX_AIOCB_PROACTOR*/

int 
main (int, char *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "This example is multithreaded version of test_timeout_st.cpp\n"
              "This doesnt work on this platform !!!\n"));
  return 1;
}

#endif /*  ACE_WIN32 && !ACE_HAS_WINCE || ACE_HAS_AIO_CALLS && !ACE_POSIX_AIOCB_PROACTOR*/
