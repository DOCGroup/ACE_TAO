// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Priority_Task_Test.cpp
//
// = DESCRIPTION
//      This is a simple test to illustrate the priority mechanism of
//      ACE Tasks.
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#include "ace/Task.h"
#include "ace/Sched_Params.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

class Priority_Task : public ACE_Task<ACE_MT_SYNCH>
// = TITLE
//   A simple Task that runs itself a different priorities.
//
// = DESCRIPTION
//   This task uses the void* argument on open to run the svc() method
//   at a different priority. The point is execise the thread priority
//   features of ACE.
{
public:
  Priority_Task (void);
  // The constructor

  int open (void *);
  // Receives the priority and run svc() on a separate thread at that
  // priority.

  int svc (void);
  // Runs on a separate thread an checks the priority.

  int succeeded (void) { return error_ == 0; }
  // Returns 1 if priority was set properly, 0 otherwise.

private:
  int priority_;
  u_int error_;
};

Priority_Task::Priority_Task (void)
  : ACE_Task<ACE_MT_SYNCH> (ACE_Thread_Manager::instance ()),
    priority_ (0),
    error_ (0)
{
}

int
Priority_Task::open (void *arg)
{
  this->priority_ = *(int *) arg;

  long flags = THR_NEW_LWP;

  // To get FIFO scheduling with PTHREADS.
  flags |= THR_SCHED_FIFO;

  // Become an active object.
  if (this->activate (flags, 1, 0, this->priority_) == -1)
    {
      // On Linux, only the superuser can set the policy to other than
      // ACE_SCHED_OTHER.  But with ACE_SCHED_OTHER, there is only 1
      // thread priority value, 0.  So, let the superuser run an
      // interesting test, but for other users use priority 0.

      ACE_DEBUG ((LM_DEBUG, "(%t) task activation at priority %d with flags "
                            "%ld failed; retry at priority 0 with with flags "
                            "%ld\n",
                  this->priority_, flags, THR_NEW_LWP));

      flags = THR_NEW_LWP;
      this->priority_ = 0;

      if (this->activate (flags, 1, 1, this->priority_) == -1)
        ACE_DEBUG ((LM_ERROR, "(%t) task activation at priority 0 failed, exiting!\n%a", -1));
    }

  return 0;
}

int
Priority_Task::svc (void)
{
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "getprio failed"), -1);

  if (prio == this->priority_)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) actual prio of %d equals desired priority\n",
                  prio));
    }
  else
    {
      ACE_DEBUG ((LM_ERROR, "(%t) actual prio = %d, desired priority_ = %d!\n",
                  prio, this->priority_));
      ++error_;
    }

  return 0;
}

#endif /* ACE_HAS_THREADS */

int
main (int, char *[])
{
  ACE_START_TEST ("Priority_Task_Test");

#if defined (ACE_HAS_THREADS)

  Priority_Task tasks[ACE_MAX_ITERATIONS];

  int status = 0;
  int i;

  // Spawn off ACE_MAX_ITERATIONS of tasks, passing each one their
  // iteration number as their priority.

  // NOTE:  on Solaris, for example, this requests the min FIFO
  // priority.  But, this test doesn't use the Realtime scheduling
  // class.  The FIFO priorities are used because they're all nonnegative.

  ACE_Sched_Priority_Iterator priority (ACE_SCHED_FIFO,
                                        ACE_SCOPE_THREAD);

  for (i = 0; i < ACE_MAX_ITERATIONS; i++)
    {
      int p = priority.priority ();
      tasks[i].open ((void *) &p);

      // If there are more priorities get the next one...
      if (priority.more ())
        {
          priority.next ();
        }
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) %d tasks spawned, wait for them to exit . . .\n",
                        ACE_MAX_ITERATIONS));

  // Wait for all tasks to exit.
  ACE_Thread_Manager::instance ()->wait ();

  for (i = 0; i < ACE_MAX_ITERATIONS; i++)
    {
      if (! tasks[i].succeeded ())
        {
          ++status;
          break;
        }
    }

#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return status;
}
