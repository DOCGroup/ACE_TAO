// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/Threads/
//
// = FILENAME
//    task_five.cpp
//
// = DESCRIPTION
//   Stress testing thread creation and thread cancellation using
//   ACE_Task.
//
// = AUTHOR
//    Author: Detlef Becker <Detlef.Becker@med.siemens.de>
//
// ============================================================================


#include "ace/OS_main.h"
#include "ace/Thread_Manager.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"

static const int DEFAULT_TASKS = 100;
static const int DEFAULT_ITERATIONS = 10;

// Default stack size
static size_t default_stack_size =
#if defined (ACE_WIN32)
    0;
#else
    8192;
#endif /* ACE_WIN32 */
  u_int loop_count = 0;
  u_int error_count = 0;

class Test_Task : public ACE_Task<ACE_SYNCH>
{
public:
  Test_Task (ACE_Thread_Manager * = ACE_Thread_Manager::instance ());
  ~Test_Task (void) {};

  //FUZZ: disable check_for_lack_ACE_OS
  int open (void * = 0);
  int svc (void);
  int close (u_long);

  int shutdown (void);
  //FUZZ: enable check_for_lack_ACE_OS

  int synch (void);
};

Test_Task::Test_Task (ACE_Thread_Manager *thrmgr)
  : ACE_Task<ACE_SYNCH> (thrmgr)
{
}

int
Test_Task::open (void *)
{
  return this->activate (0,
                         1,
                         0,
                         ACE_DEFAULT_THREAD_PRIORITY,
                         -1,
                         0,
                         0,
                         0,
                         &default_stack_size);
}

int
Test_Task::svc (void)
{
  while (thr_mgr_->testcancel (ACE_OS::thr_self ()) == 0)
    // Sleep for 350 msecs.
    ACE_OS::sleep (ACE_Time_Value (0, 350000));

  return 0;
}

int
Test_Task::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) closing down\n"));
  return 0;
}

int
Test_Task::shutdown (void)
{
  return thr_mgr_->cancel_grp (grp_id_);
}

int
Test_Task::synch (void)
{
  return thr_mgr_->wait_grp (grp_id_);
}

static void
work (ACE_Thread_Manager *thr_mgr,
      int n_tasks,
      size_t stack_size)
{
  ACE_UNUSED_ARG (stack_size);

  int i;
  Test_Task *task_array;

  ACE_NEW (task_array,
           Test_Task[n_tasks]);

  ACE_DEBUG ((LM_DEBUG,
              "Opening Tasks, loop count = %d, error count = %d\n",
              loop_count,
              error_count));

  for (i = 0;
       i < n_tasks;
       i++)
    task_array[i].open ();

  ACE_OS::sleep (1);

  ACE_DEBUG ((LM_DEBUG,
              "Cancelling Tasks, loop count = %d, error count = %d\n",
              loop_count,
              error_count));

  for (i = 0; i < n_tasks; i++)
    task_array[i].shutdown ();

  ACE_DEBUG ((LM_DEBUG,
              "Synching Tasks, loop count = %d, error count = %d\n",
              loop_count,
              error_count));

  for (i = 0;

       i < n_tasks; i++)
    if (-1 == task_array[i].synch ())
      {
        ACE_ERROR ((LM_ERROR,
                    "Error in synch! loop count = %d, error count = %d\n",
                    loop_count,
                    error_count));
        error_count++;
      }

  ACE_DEBUG ((LM_DEBUG,
              "thr_mgr->wait ();! loop count = %d, error count = %d\n",
              loop_count,
              error_count));

  // Wait for all the threads to finish.
  thr_mgr->wait ();

  delete [] task_array;
  loop_count++;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  size_t stack_size = argc > 1 ? ACE_OS::atoi (argv[1]) : default_stack_size;
  const int n_tasks = argc > 2 ? ACE_OS::atoi (argv[2]) : DEFAULT_TASKS;
  u_int iterations = argc > 3 ? ACE_OS::atoi (argv[3]) : DEFAULT_ITERATIONS;

  for (u_int i = 0; i < iterations; i++)
    work (ACE_Thread_Manager::instance (),
          n_tasks,
          stack_size);

  return 0;
}
