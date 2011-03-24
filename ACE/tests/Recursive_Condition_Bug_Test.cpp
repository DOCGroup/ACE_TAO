
//=============================================================================
/**
 *  @file    Recursive_Condition_Bug_Test.cpp
 *
 *  $Id$
 *
 *    This test program validates the functionality of the
 *    ACE_Condition<ACE_Recursive_Thread_Mutex> template
 *    specialization when combined with the
 *    ACE_Thread_Timer_Queue_Adapter on Win32 and Posix pthreads.
 *    It was added to test for bugs with the ACE_OS recursive
 *    condition implementation.
 *
 *
 *  @author Leonid Kvetnyi <leonidk@nice.com> and Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Task_T.h"
#include "ace/Activation_Queue.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"
#include "ace/Condition_Recursive_Thread_Mutex.h"

#if defined (ACE_HAS_THREADS)

// Number of iterations for the performance tests.
static int max_iterations = 30;

typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> Thread_Timer_Queue;

class Test_Handler;

class Test_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  virtual int svc (void)
  {
    while (--max_iterations > 0)
      {
        // dequeue the next object
        ACE_Message_Block * mb = 0;

        if (this->getq (mb) == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%t) %p\n"),
                             ACE_TEXT ("getq failed")),
                            -1);

        Test_Handler *handler = reinterpret_cast<Test_Handler *> (mb->base ());
        mb->release ();

        ACE_Time_Value timeout = ACE_OS::gettimeofday () + ACE_Time_Value (1, 0);

        if (timer_queue_.schedule (reinterpret_cast<ACE_Event_Handler *> (handler),
                                   this,
                                   timeout) == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%t) %p\n"),
                             ACE_TEXT ("schedule failed")),
                            -1);

        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("(%t) %d iteration(s) remain\n"),
                    max_iterations));
      }

    timer_queue_.deactivate ();
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("(%t) leaving the Test_task\n")));
    return 0;
  }

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void * = 0)
  {
  //FUZZ: enable check_for_lack_ACE_OS
    if (ACE_Task<ACE_MT_SYNCH>::activate (THR_NEW_LWP, 1) != 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) %p\n"),
                         ACE_TEXT ("Test_Task::activate")),
                        -1);
    if (0 != timer_queue_.activate ())
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) %p\n"),
                         ACE_TEXT ("Test_Task::queue activate")),
                        -1);
    return 0;
  }

private:
  Thread_Timer_Queue timer_queue_;
};

class Test_Handler : public ACE_Event_Handler
{
public:
  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *arg)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Test_Handler::handle_timeout\n")));

    void *nc_arg = const_cast<void *> (arg);
    Test_Task *test_task =
      reinterpret_cast<Test_Task *> (nc_arg);
    ACE_Message_Block *mb = 0;
    ACE_NEW_MALLOC_RETURN (mb,
                           static_cast<ACE_Message_Block *> (ACE_Allocator::instance()->malloc (sizeof (ACE_Message_Block))),
                           ACE_Message_Block (sizeof (*this),    // size
                                              ACE_Message_Block::MB_DATA, // type
                                              0,       // cont
                                              (char *) this,     // data
                                              0,
                                              0,
                                              ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY,
                                              ACE_Time_Value::zero,
                                              ACE_Time_Value::max_time,
                                              0,
                                              ACE_Allocator::instance()),     // data
                           -1);

    test_task->putq (mb);
    return 0;
  }
};

#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Recursive_Condition_Bug_Test"));

#if defined (ACE_HAS_THREADS)

  // Timer queue usage.

  Test_Handler handler;
  Test_Task task;

  if (0 != task.open ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("open")),
                      -1);

  ACE_Message_Block *mb = 0;
  ACE_NEW_MALLOC_RETURN (mb,
                         static_cast<ACE_Message_Block *> (ACE_Allocator::instance()->malloc (sizeof (ACE_Message_Block))),
                         ACE_Message_Block (sizeof (handler),    // size
                                            ACE_Message_Block::MB_DATA, // type
                                            0,       // cont
                                            (char *) &handler,
                                            0,
                                            0,
                                            ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY,
                                            ACE_Time_Value::zero,
                                            ACE_Time_Value::max_time,
                                            0,
                                            ACE_Allocator::instance()),     // data
                          -1);

  if (-1 == task.putq (mb))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("putq")),
                      -1);

  if (ACE_Thread_Manager::instance ()->wait () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "wait on Thread_Manager failed"),
                      -1);
#else
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("ACE doesn't support recursive condition variables on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}

