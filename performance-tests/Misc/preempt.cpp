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
//      This is a simple test to illustrate OS thread preemption.  One
//      ore more high priority threads periodically (every half
//      second, by default) reads the clock.  They use select () to
//      block for that duration.  Meanwhile, a low priority thread
//      continually chews up the CPU.  Without preemption, the high
//      priority thread won't have a chance to read the clock in a
//      timely manner.
//
//      At the end of the test, the actual clock read intervals by the
//      high priority task(s) are printed out.  With proper
//      preemption, the intervals should correspond to the requested
//      clock read interval.
//
//      There is a -y option for the low priority thread to periodically
//      yield.  It shouldn't be necessary to use that option, if preemption
//      is supported.  It's a handy option for testing.
//
// = AUTHOR
//    David L. Levine
//
// ============================================================================

#include "ace/Task.h"
#include "ace/Sched_Params.h"
#include "ace/Get_Opt.h"

#if defined (ACE_HAS_THREADS)

static const char usage [] = "[-? |\n"
                             "       [-c <iterations, default 10>]\n"
                             "       [-n <high priority threads, default 1>\n"
                             "       [-p <read period, default 500000 usec>\n"
                             "       [-y to yield the low priority thread\n"
                             "[<iterations>]]";

// Configuration options.
u_int iterations = 10;
u_int high_priority_tasks = 1;
u_long read_period = 500000; /* usec */
u_int low_yield = 0;

// To permit calculation of relative times.
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
  ~High_Priority_Task (void);

  int open (void *);
  int svc (void);
  int done () const { return done_; }
  void print_times () const;

private:
  int priority_;
  int done_;
  u_long *time_;
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
  ACE_NEW (time_, u_long[iterations]);
}

High_Priority_Task::~High_Priority_Task (void)
{
  delete [] time_;
  time_ = 0;
}

int
High_Priority_Task::open (void *)
{
  long flags = THR_BOUND | THR_SCHED_FIFO;

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

  ACE_Time_Value pause (0, read_period);

  for (u_int i = 0; i < iterations; ++i)
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
  for (u_int i = 0; i < iterations; ++i)
    {
      ACE_OS::printf ("  iteration %u, time %lu msec\n",
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
  long flags = THR_BOUND | THR_SCHED_FIFO;

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
         ! high_priority_task_.done () &&
         iterations < interval * 1000000 / 40.2;
       ++iterations)
    {
      u_long n = 1279ul;  /* takes about 40.2 usecs on a 168 MHz Ultra2 */
      ACE::is_prime (n,
                     2ul /* min_factor */,
                     n/2 /* max_factor */);

      if (low_yield) ACE_OS::thr_yield ();
    }

  ACE_OS::printf ("%lu iterations\n", iterations);

  return 0;
}

#endif /* ACE_HAS_THREADS */


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function get_options
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static
unsigned int
get_options (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "c:n:p:y?");
  int opt;
  while ((opt = get_opt ()) != EOF) {
    switch (opt) {
    case 'c':
      if (ACE_OS::atoi (get_opt.optarg) >= 2)
        {
          iterations = ACE_OS::atoi (get_opt.optarg);
        }
      else
        {
          ACE_DEBUG ((LM_ERROR, "%n: iterations must be >= 2\n"));
          return 1;
        }
      break;
    case 'n':
      if (ACE_OS::atoi (get_opt.optarg) >= 1)
        {
          high_priority_tasks = ACE_OS::atoi (get_opt.optarg);
        }
      else
        {
          ACE_DEBUG ((LM_ERROR, "%n: number of high priority threads "
                                "must be >= 1\n"));
          return 1;
        }
      break;
    case 'p':
      if (ACE_OS::atoi (get_opt.optarg) > 0)
        {
          read_period = ACE_OS::atoi (get_opt.optarg);
        }
      else
        {
          ACE_DEBUG ((LM_ERROR, "%n: read period > 0\n"));
          return 1;
        }
      break;
    case 'y':
      low_yield = 1;
      break;
    case '?':
      ACE_DEBUG ((LM_ERROR, "usage: %n %s\n%a", usage, 0));
      break;
    default:
      ACE_DEBUG ((LM_ERROR, "%n: unknown arg, %c\n", opt));
      ACE_DEBUG ((LM_ERROR, "usage: %n %s\n", usage));
      return 1;
    }
  }

  switch (argc - get_opt.optind) {
  case 0:
    // use default number of iterations
    break;
  default:
    ACE_DEBUG ((LM_ERROR, "%n: too many arguments\n"));
    ACE_DEBUG ((LM_ERROR, "usage: %n %s\n", usage));
    return 1;
  }

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function main
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int
main (int argc, char *argv[])
{
#if defined (ACE_HAS_THREADS)

  u_int i;

  if (get_options (argc, argv)) ACE_OS::exit (-1);

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
          ACE_OS::perror ("preempt; ACE_OS::sched_params ()");
          ACE_OS::exit (-1);
        }
    }

  High_Priority_Task *high_priority_task;
  ACE_NEW_RETURN (high_priority_task, High_Priority_Task [high_priority_tasks],
                  1);

  Low_Priority_Task low_priority_task (high_priority_task[0]);

  // Save the start time, so that deltas can be displayed later.
  starttime = ACE_OS::gethrtime ();

  // Spawn the threads . . .
  for (i = 0; i < high_priority_tasks; ++i)
  {
    high_priority_task[i].open (0);
  }
  low_priority_task.open (0);

  ACE_DEBUG ((LM_DEBUG, "(%t) main (), wait for threads to exit . . .\n"));

  // Wait for all threads to exit.
  ACE_Thread_Manager::instance ()->wait ();

  // Display the time deltas.  They should be about a half second apart.
  for (i = 0; i < high_priority_tasks; ++i)
  {
    ACE_OS::printf ("High priority task %u:\n", i + 1);
    high_priority_task[i].print_times ();
  }

  delete [] high_priority_task;

#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */

  return 0;
}
