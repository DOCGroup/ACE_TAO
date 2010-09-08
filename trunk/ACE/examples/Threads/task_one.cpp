// $Id$

// This test program illustrates how the ACE barrier synchronization
// mechanisms work in conjunction with the ACE_Task and the
// ACE_Thread_Manager.  It is instructive to compare this with the
// test_barrier.cpp test to see how they differ.

#include "ace/OS_main.h"
#include "ace/Task.h"
#include "ace/Service_Config.h"



#if defined (ACE_HAS_THREADS)

#include "ace/Task.h"
#include "ace/Barrier.h"

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
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));
}

// Iterate <n_iterations> time printing off a message and "waiting"
// for all other threads to complete this iteration.

int
Barrier_Task::svc (void)
{
  // Note that the ACE_Task::svc_run() method automatically adds us to
  // the Thread_Manager when the thread begins.

  for (int iterations = 1;
       iterations <= this->n_iterations_;
       iterations++)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) in iteration %d\n", iterations));

      // Block until all other threads have waited, then continue.
      this->barrier_.wait ();
    }

  // Note that the ACE_Task::svc_run() method automatically removes us
  // from the Thread_Manager when the thread exits.

  return 0;
}

// Default number of threads to spawn.
static const int DEFAULT_ITERATIONS = 5;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int n_threads = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_THREADS;
  int n_iterations = argc > 2 ? ACE_OS::atoi (argv[2]) : DEFAULT_ITERATIONS;

  Barrier_Task barrier_task (ACE_Thread_Manager::instance (),
                             n_threads,
                             n_iterations);

  // Wait for all the threads to reach their exit point.
  ACE_Thread_Manager::instance ()->wait ();

  ACE_DEBUG ((LM_DEBUG, "(%t) done\n"));
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
