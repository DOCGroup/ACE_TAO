// $Id$

// Exercise more tests for the ACE Tasks.  This test can spawn off
// zillions of tasks and then wait for them using both polling and the
// ACE Thread Manager.

#include "ace/Task.h"

#include "ace/Service_Config.h"
#include "ace/Synch.h"

#if defined (ACE_HAS_THREADS)

typedef ACE_Atomic_Op<ACE_Thread_Mutex, int> ATOMIC_INT;

static u_long zero = 0;
static ATOMIC_INT task_count (zero);
static ATOMIC_INT max_count (zero);
static ATOMIC_INT wait_count (zero);

static int n_threads = 0;

// Default number of tasks.
static const int default_threads = ACE_DEFAULT_THREADS;

// Default number of times to run the test.
static const int default_iterations = 1000;

class Task_Test : public ACE_Task<ACE_MT_SYNCH>
{
public:
  virtual int open (void *args = 0);
  virtual int close (u_long flags = 0);
  virtual int svc (void);

private:
  static ACE_Thread_Mutex lock_;
};

ACE_Thread_Mutex Task_Test::lock_;

int 
Task_Test::open (void *)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, Task_Test::lock_, -1);

  task_count++;
  ACE_DEBUG ((LM_DEBUG, "(%t) creating Task_Test, task count = %d\n", 
	      (u_long) task_count));

  return this->activate (THR_BOUND);
}

int 
Task_Test::close (u_long)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, Task_Test::lock_, -1);

  task_count--;
  ACE_DEBUG ((LM_DEBUG, "(%t) destroying Task_Test, task count = %d\n", 
	      (u_long) task_count));
  wait_count--;
// delete this;
  return 0;
}

int 
Task_Test::svc (void)
{
  wait_count++;
  max_count++;

  ACE_DEBUG ((LM_DEBUG, "(%t) svc: waiting\n"));

  for (;;)
    if (max_count >= n_threads)
      break;
    else
      ACE_Thread::yield ();

  ACE_DEBUG ((LM_DEBUG, "(%t) svc: finished waiting\n"));
  return 0;
}

int 
main (int argc, char *argv[])
{
  n_threads = argc > 1 ? ACE_OS::atoi (argv[1]) : default_threads;
  int n_iterations = argc > 2 ? ACE_OS::atoi (argv[2]) : default_iterations;

  Task_Test **task_array = new Task_Test *[n_threads];

  for (int i = 1; i <= n_iterations; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) iteration = %d, max_count %d\n", 
		  i, (u_long) max_count));
      max_count = 0;

      ACE_DEBUG ((LM_DEBUG, "(%t) starting %d task%s\n", 
		  n_threads, n_threads == 1 ? "" : "s"));

      // Launch the new tasks.
      for (int j = 0; j < n_threads; j++)
	{
	  task_array[j] = new Task_Test;
	  // Activate the task, i.e., make it an active object.
	  task_array[j]->open ();
	}

      // Wait for initialization to kick in.
      while (max_count == 0)
	ACE_Thread::yield ();
      
      ACE_DEBUG ((LM_DEBUG, "(%t) waiting for threads to finish\n"));

      // Wait for the threads to finish this iteration.
      while (max_count != n_threads && wait_count != 0)
	ACE_Thread::yield ();

      ACE_DEBUG ((LM_DEBUG, "(%t) iteration %d finished, max_count %d, wait_count %d, waiting for tasks to exit\n",
		  i, (u_long) max_count, (u_long) wait_count));

      // Wait for all the tasks to exit.
      ACE_Service_Config::thr_mgr ()->wait ();

      // Delete the existing tasks.
      for (int k = 0; k < n_threads; k++)
	delete task_array[k];
    }

  delete [] task_array;

  ACE_DEBUG ((LM_DEBUG, "(%t) shutting down the test\n"));
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Atomic_Op<ACE_Thread_Mutex, int>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#else
int 
main (int, char *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
