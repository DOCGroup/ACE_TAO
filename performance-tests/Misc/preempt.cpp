// $Id$

// ============================================================================
//
// = LIBRARY
//    performance-tests/Misc
//
// = FILENAME
//    preempt.cpp
//
// = DESCRIPTION
//      This is a simple test to illustrate OS thread preemption.
//      A high priority thread periodically (every half second) reads the
//      clock.  It uses select () to block for that duration.  Meanwhile,
//      a low priority thread continually chews up the CPU.  Without
//      preemption, the high priority thread won't have a chance to read
//      the clock in a timely manner.
//
// = AUTHOR
//    David L. Levine
//
// ============================================================================

#include "ace/Task.h"
#include "ace/Sched_Params.h"

#if defined (ACE_HAS_THREADS)

ACE_hrtime_t starttime;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class High_Priority_Task
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class High_Priority_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  High_Priority_Task (void);

  int open (void *);
  int svc (void);
  int done () const { return done_; }
  void print_times () const;

private:
  int priority_;
  int done_;

  enum { ITERATIONS = 10 };

  u_long time_ [ITERATIONS];
};

High_Priority_Task::High_Priority_Task (void)
  : ACE_Task<ACE_MT_SYNCH> (ACE_Thread_Manager::instance ()),
    priority_ (ACE_Sched_Params::next_priority (
                 ACE_SCHED_FIFO,
                 ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                                 ACE_SCOPE_THREAD),
                 ACE_SCOPE_THREAD)),
    done_ (0)
{
}

int
High_Priority_Task::open (void *)
{
  long flags = THR_BOUND;

  // To get FIFO scheduling with PTHREADS.
  flags |= THR_SCHED_FIFO;

  // Become an active object.
  if (this->activate (flags, 1, 0, this->priority_) == -1)
    {
      ACE_DEBUG ((LM_ERROR, "(%t) task activation failed, exiting!\n%a", -1));
    }

  return 0;
}

int
High_Priority_Task::svc (void)
{
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "getprio failed"), -1);

  ACE_DEBUG ((LM_DEBUG, "(%t) High: prio = %d, priority_ = %d\n",
	      prio, this->priority_));
  ACE_ASSERT (this->priority_ == prio);

  ACE_Time_Value pause (0, 500000);

  for (u_int i = 0; i < ITERATIONS; ++i)
    {
      time_ [i] = (u_long) ((ACE_OS::gethrtime () - starttime)/1000000ul);
      ACE_OS::select (0, 0, 0, 0, &pause);
    }

  done_ = 1;

  return 0;
}

void
High_Priority_Task::print_times () const
{
  for (u_int i = 0; i < ITERATIONS; ++i)
    {
      ACE_OS::fprintf (stderr, "High: iteration %u, time %lu msec\n",
                       i, time_ [i]);
    }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Low_Priority_Task
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class Low_Priority_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Low_Priority_Task (const High_Priority_Task &);

  int open (void *);
  int svc (void);

private:
  int priority_;
  const High_Priority_Task &high_priority_task_;
};

Low_Priority_Task::Low_Priority_Task (
  const High_Priority_Task &high_priority_task)
  : ACE_Task<ACE_MT_SYNCH> (ACE_Thread_Manager::instance ()),
    priority_ (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                               ACE_SCOPE_THREAD)),
    high_priority_task_ (high_priority_task)
{
}

int
Low_Priority_Task::open (void *)
{
  long flags = THR_BOUND;

  // To get FIFO scheduling with PTHREADS.
  flags |= THR_SCHED_FIFO;

  // Become an active object.
  if (this->activate (flags, 1, 0, this->priority_) == -1)
    {
      ACE_DEBUG ((LM_ERROR, "(%t) task activation failed, exiting!\n%a", -1));
    }

  return 0;
}

int
Low_Priority_Task::svc (void)
{
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "getprio failed"), -1);

  ACE_DEBUG ((LM_DEBUG, "(%t) Low: prio = %d, priority_ = %d\n",
	      prio, this->priority_));
  ACE_ASSERT (this->priority_ == prio);

  const u_int interval = 1; /* seconds */
  u_long iterations = 0;

  // Chew up CPU for the entire interval.
  for (;
       //       ! high_priority_task_.done () &&
         iterations < interval * 1000000 / 40.2;
       ++iterations)
    {
      u_long n = 1279ul;  /* takes about 40.2 usecs on a 168 MHz Ultra2 */
      ACE::is_prime (n,
                     2ul /* min_factor */,
                     n/2 /* max_factor */);
    }

  ACE_OS::fprintf (stderr, "%lu iterations\n", iterations);

  return 0;
}

#endif /* ACE_HAS_THREADS */


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function main
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int
main (int, char *[])
{
#if defined (ACE_HAS_THREADS)

  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
          ACE_Sched_Params::priority_min (ACE_SCHED_FIFO),
          ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_MAX, "preempt: user is not superuser, "
                              "so remain in time-sharing class\n"));
        }
      else
        {
          ACE_OS::perror ("context_switch_time");
          ACE_OS::exit (-1);
        }
    }

  High_Priority_Task high_priority_task;
  Low_Priority_Task low_priority_task (high_priority_task);

  // Save the start time, so that deltas can be displayed later.
  starttime = ACE_OS::gethrtime ();

  // Spawn the threads . . .
  low_priority_task.open (0);
  high_priority_task.open (0);

  ACE_DEBUG ((LM_DEBUG, "(%t) main (), wait for threads to exit . . .\n"));

  // Wait for all threads to exit.
  ACE_Thread_Manager::instance ()->wait ();

  // Display the time deltas.  They should be about a half second apart.
  high_priority_task.print_times ();

#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */

  return 0;
}
