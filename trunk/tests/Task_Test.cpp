// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Task_Test.cpp
//
// = DESCRIPTION
//      This test program illustrates how the ACE barrier
//      synchronization mechanisms work in conjunction with the
//      <ACE_Task> and the <ACE_Thread_Manager>.  This also illustrates
//      how the <ACE_Thread_Hook> mechanism works.
//
// = AUTHOR
//    Prashant Jain <pjain@cs.wustl.edu> and
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Task.h"
#include "ace/Thread_Hook.h"

ACE_RCSID(tests, Task_Test, "$Id$")

#if defined (ACE_HAS_THREADS)

class My_Thread_Hook : public ACE_Thread_Hook
{
public:
  virtual void *start (ACE_THR_FUNC func,
                       void *arg);
};

class Barrier_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Barrier_Task (ACE_Thread_Manager *thr_mgr,
                int n_threads,
                int n_iterations);

  virtual int svc (void);
  // Iterate <n_iterations> time printing off a message and "waiting"
  // for all other threads to complete this iteration.

private:
  ACE_Barrier barrier_;
  // Reference to the tester barrier.  This controls each iteration of
  // the tester function running in every thread.

  int n_iterations_;
  // Number of iterations to run.
};

Barrier_Task::Barrier_Task (ACE_Thread_Manager *thr_mgr,
                            int n_threads,
                            int n_iterations)
  : ACE_Task<ACE_MT_SYNCH> (thr_mgr),
    barrier_ (n_threads),
    n_iterations_ (n_iterations)
{
  // Create worker threads.
  if (this->activate (THR_NEW_LWP, n_threads) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("activate failed")));
}

void *
My_Thread_Hook::start (ACE_THR_FUNC func,
                       void *arg)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) starting the thread!\n")));
  return (func) (arg);
}

// Iterate <n_iterations> time printing off a message and "waiting"
// for all other threads to complete this iteration.

int
Barrier_Task::svc (void)
{
  for (int iterations = 1;
       iterations <= this->n_iterations_;
       iterations++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) in iteration %d\n"),
                  iterations));

      // Block until all other threads have waited, then continue.
      this->barrier_.wait ();
    }

  // Note that the <ACE_Task::svc_run> method automatically removes us
  // from the Thread_Manager when the thread exits.

  return 0;
}

#endif /* ACE_HAS_THREADS */

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Task_Test"));

#if defined (ACE_HAS_THREADS)
  // Set the thread hook!
  ACE_Thread_Hook::thread_hook (new My_Thread_Hook);

  int n_threads = ACE_MAX_THREADS;
  int n_iterations = ACE_MAX_ITERATIONS;

  Barrier_Task barrier_task (ACE_Thread_Manager::instance (),
                             n_threads,
                             n_iterations);

  ACE_Thread_Manager::instance ()->wait ();
  // Cleanup the thread hook so it doesn't leak.
  delete ACE_Thread_Hook::thread_hook ();
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
