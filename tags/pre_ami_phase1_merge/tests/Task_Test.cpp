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
//      ACE_Task and the ACE_Thread_Manager.
//
// = AUTHOR
//    Prashant Jain and Doug C. Schmidt
//
// ============================================================================

#include "test_config.h"
#include "ace/Task.h"

ACE_RCSID(tests, Task_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS)

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
  // Reference to the tester barrier.  This controls each
  // iteration of the tester function running in every thread.

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
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("%p\n"), ASYS_TEXT ("activate failed")));
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
      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) in iteration %d\n"), iterations));

      // Block until all other threads have waited, then continue.
      this->barrier_.wait ();
    }

  // Note that the ACE_Task::svc_run() method automatically removes us
  // from the Thread_Manager when the thread exits.

  return 0;
}

#endif /* ACE_HAS_THREADS */

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Task_Test"));

#if defined (ACE_HAS_THREADS)
  int n_threads = ACE_MAX_THREADS;
  int n_iterations = ACE_MAX_ITERATIONS;

  Barrier_Task barrier_task (ACE_Thread_Manager::instance (),
                             n_threads,
                             n_iterations);

  ACE_Thread_Manager::instance ()->wait ();

#else
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
