
//=============================================================================
/**
 *  @file    preempt.cpp
 *
 *    This is a simple test to illustrate OS thread preemption.  One
 *    ore more high priority threads periodically (every half
 *    second, by default) reads the clock.  They use select () to
 *    block for that duration.  Meanwhile, a low priority thread
 *    continually chews up the CPU.  Without preemption, the high
 *    priority thread won't have a chance to read the clock in a
 *    timely manner.
 *
 *    At the end of the test, the actual clock read intervals by the
 *    high priority task(s) are printed out.  With proper
 *    preemption, the intervals should correspond to the requested
 *    clock read interval.
 *
 *    There is a -y option for the low priority thread to periodically
 *    yield.  It shouldn't be necessary to use that option, if preemption
 *    is supported.  It's a handy option for testing.
 *
 *  @author David L. Levine
 */
//=============================================================================

#include "ace/OS_main.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Sched_Params.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_select.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_unistd.h"
#include <memory>
#include <new>
#include <atomic>

#if defined (ACE_HAS_THREADS) || ! defined (ACE_LACKS_FORK)

static char const usage [] = "[-? |\n"
#if defined (ACE_HAS_THREADS)
                            "       [-f use fork instead of spawn]\n"
#endif /* ACE_HAS_THREADS */
                            "       [-h <high pri iterations, default 10>]\n"
                            "       [-l <low pri iterations, default 25000>]\n"
                            "       [-n <high priority threads, default 1>]\n"
                            "       [-p <read period, default 500000 usec>]\n"
                            "       [-y to yield the low priority thread]]\n";

// Configuration options.
#if defined (ACE_HAS_THREADS)
static bool use_fork {};
#else  /* ! ACE_HAS_THREADS */
static bool use_fork {true};
#endif /* ! ACE_HAS_THREADS */
static u_int high_iterations {10};
static u_int high_priority_tasks {1};
static u_int low_iterations {25000};  /* Big enough to keep the low priority task
                                        cranking for a while */
static u_long read_period {500000}; /* usec */
static bool low_yield {};

// To permit calculation of relative times.
static ACE_hrtime_t starttime {};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class High_Priority_Task
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class High_Priority_Task : public ACE_Task<ACE_SYNCH>
{
public:
  High_Priority_Task ();
  ~High_Priority_Task () override = default;

  //FUZZ: disable check_for_lack_ACE_OS
  int open (void *) override;
  //FUZZ: enable check_for_lack_ACE_OS

  int svc () override;
  bool done () const { return done_.load (std::memory_order_relaxed); }
  bool valid () const { return static_cast<bool> (time_); }
  void print_times () const;

private:
  int priority_;
  std::atomic<bool> done_ {};
  std::unique_ptr<u_long[]> time_;
  High_Priority_Task (High_Priority_Task const &) = delete;
  High_Priority_Task &operator= (High_Priority_Task const &) = delete;
  High_Priority_Task (High_Priority_Task &&) = delete;
  High_Priority_Task &operator= (High_Priority_Task &&) = delete;
};

High_Priority_Task::High_Priority_Task ()
  : ACE_Task<ACE_SYNCH> (ACE_Thread_Manager::instance ()),
    priority_ (ACE_Sched_Params::next_priority (
                 ACE_SCHED_FIFO,
                 ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                                 ACE_SCOPE_THREAD),
                 ACE_SCOPE_THREAD)),
    time_ (new (std::nothrow) u_long[high_iterations] {})
{
  if (!this->time_)
    {
      errno = ENOMEM;
      return;
    }
}

int
High_Priority_Task::open (void *)
{
  if (use_fork)
    {
      ACE_hthread_t thr_handle {};
      ACE_Thread::self (thr_handle);

      if (ACE_Thread::setprio (thr_handle, priority_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "setprio failed"), -1);

      return svc ();
    }
  else
    {
      long const flags = THR_BOUND | THR_NEW_LWP | THR_SCHED_FIFO;

      // Become an active object.
      if (this->activate (flags, 1, 0, this->priority_) == -1)
        {
          ACE_DEBUG ((LM_ERROR, "(%P|%t) task activation failed, exiting!\n"));
          ACE_OS::exit (1);
        }

      return 0;
    }
}

int
High_Priority_Task::svc ()
{
  ACE_hthread_t thr_handle {};
  ACE_Thread::self (thr_handle);
  int prio {};

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "getprio failed"), -1);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) High: prio = %d, priority_ = %d\n",
              prio, this->priority_));

  ACE_ASSERT (this->priority_ == prio);

  ACE_Time_Value pause (0, read_period);

  for (u_int i = 0; i < high_iterations; ++i)
    {
      time_ [i] = static_cast<u_long> ((ACE_OS::gethrtime () - starttime) /
                                       static_cast<ACE_UINT32> (1000000u));
      ACE_OS::select (0, nullptr, nullptr, nullptr, std::addressof (pause));
    }

  done_.store (true, std::memory_order_relaxed);

  return 0;
}

void
High_Priority_Task::print_times () const
{
  for (u_int i = 0; i < high_iterations; ++i)
    {
      ACE_DEBUG ((LM_INFO, "  iteration %u, time %u msec\n",
                           i, time_ [i]));
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Low_Priority_Task
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class Low_Priority_Task : public ACE_Task<ACE_SYNCH>
{
public:
  Low_Priority_Task () = delete;
  explicit Low_Priority_Task (High_Priority_Task const&);

  //FUZZ: disable check_for_lack_ACE_OS
  int open (void *) override;
  //FUZZ: enable check_for_lack_ACE_OS

  int svc () override;

private:
  int priority_;
  High_Priority_Task const& high_priority_task_;
  Low_Priority_Task (Low_Priority_Task const &) = delete;
  Low_Priority_Task &operator= (Low_Priority_Task const &) = delete;
  Low_Priority_Task (Low_Priority_Task &&) = delete;
  Low_Priority_Task &operator= (Low_Priority_Task &&) = delete;
};

Low_Priority_Task::Low_Priority_Task (
  High_Priority_Task const& high_priority_task)
  : ACE_Task<ACE_SYNCH> (ACE_Thread_Manager::instance ()),
    priority_ (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                               ACE_SCOPE_THREAD)),
    high_priority_task_ (high_priority_task)
{
}

int
Low_Priority_Task::open (void *)
{
  if (use_fork)
    {
      ACE_hthread_t thr_handle {};
      ACE_Thread::self (thr_handle);

      if (ACE_Thread::setprio (thr_handle, priority_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "setprio failed"), -1);

      return svc ();
    }
  else
    {
      long const flags = THR_BOUND | THR_NEW_LWP | THR_SCHED_FIFO;

      // Become an active object.
      if (this->activate (flags, 1, 0, this->priority_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) task activation failed, exiting!\n"),
                          -1);
      return 0;
    }
}

int
Low_Priority_Task::svc ()
{
  ACE_hthread_t thr_handle {};
  ACE_Thread::self (thr_handle);
  int prio {};

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "getprio failed"), -1);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Low: prio = %d, priority_ = %d\n",
              prio, this->priority_));

  ACE_ASSERT (this->priority_ == prio);

  u_long iterations {};

  // Chew up CPU for the entire interval.
  for (;
       ! high_priority_task_.done () && iterations < low_iterations;
       ++iterations)
    {
      u_long n {1279ul};  /* Takes about 40.2 usecs on a 168 MHz Ultra2. */
      ACE::is_prime (n,
                     2ul /* min_factor */,
                     n/2 /* max_factor */);

      if (low_yield)
        ACE_OS::thr_yield ();
    }

  ACE_DEBUG ((LM_INFO, "Low completed %u iterations\n",
              static_cast<u_int> (iterations)));

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function get_options
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static int
get_options (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("fh:l:n:p:y?"));
  int opt {};
  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'f':
          use_fork = true;
          break;
        case 'h':
          {
            int const value = ACE_OS::atoi (get_opt.opt_arg ());
            if (value >= 2)
              high_iterations = static_cast<u_int> (value);
            else
              ACE_ERROR_RETURN ((LM_ERROR, "%n: high iterations must be >= 2\n"), -1);
          }
          break;
        case 'l':
          {
            int const value = ACE_OS::atoi (get_opt.opt_arg ());
            if (value >= 2)
              low_iterations = static_cast<u_int> (value);
            else
              ACE_ERROR_RETURN ((LM_ERROR, "%n: low iterations must be >= 2\n"), -1);
          }
          break;
        case 'n':
          {
            int const value = ACE_OS::atoi (get_opt.opt_arg ());
            if (value >= 1)
              high_priority_tasks = static_cast<u_int> (value);
            else
              ACE_ERROR_RETURN ((LM_ERROR, "%n: number of high priority threads must be >= 1\n"), -1);
          }
          break;
        case 'p':
          {
            int const value = ACE_OS::atoi (get_opt.opt_arg ());
            if (value > 0)
              read_period = static_cast<u_long> (value);
            else
              ACE_ERROR_RETURN ((LM_ERROR, "%n: read period must be > 0\n"), -1);
          }
          break;
        case 'y':
          low_yield = true;
          break;
        case '?':
          ACE_DEBUG ((LM_ERROR, "usage: %n %s\n", usage));
          ACE_OS::exit (0);
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%n: unknown arg, %c\nusage: %n %s\n", opt, usage),
                            -1);
        }
    }

  switch (argc - get_opt.opt_ind ()) {
  case 0:
    // OK, no non-flag arguments.
    break;
  default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%n: too many arguments\nusage: %n %s\n", usage),
                        -1);
  }

  return 0;
}

#endif /* ACE_HAS_THREADS || ! ACE_LACKS_FORK */

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function main
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_LOG_MSG->open (argv[0] != nullptr ? argv[0] : ACE_TEXT("preempt"));

#if defined (ACE_HAS_THREADS) || !defined (ACE_LACKS_FORK)

  u_int i {};

  if (get_options (argc, argv))
    ACE_OS::exit (-1);

  // Enable FIFO scheduling
  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
          ACE_Sched_Params::priority_min (ACE_SCHED_FIFO),
          ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        ACE_DEBUG ((LM_MAX, "preempt: user is not superuser, "
                    "so remain in time-sharing class\n"));
      else
        ACE_ERROR_RETURN ((LM_ERROR, "%n: ACE_OS::sched_params failed\n%a"),
                          -1);
    }

  std::unique_ptr<High_Priority_Task[]> high_priority_task (
    new (std::nothrow) High_Priority_Task[high_priority_tasks]);
  if (!high_priority_task)
    {
      errno = ENOMEM;
      return 1;
    }

  for (u_int task_index {}; task_index < high_priority_tasks; ++task_index)
    if (!high_priority_task[task_index].valid ())
      ACE_ERROR_RETURN ((LM_ERROR, "Unable to allocate high-priority task timing data\n"), 1);

  Low_Priority_Task low_priority_task (high_priority_task[0]);

  if (! use_fork)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) main (), wait for threads to exit . . .\n"));
    }

  // Save the start time, so that deltas can be displayed later.
  starttime = ACE_OS::gethrtime ();

  // Spawn the threads/processes . . .
  pid_t child {};
  if (use_fork)
    {
      switch ((child = ACE_OS::fork (ACE_TEXT("preempt-low_priority_process"))))
        {
        case -1:
          ACE_ERROR ((LM_ERROR, "%p\n%a", "fork failed"));
          return -1;
        case 0: // In child
          {
            low_priority_task.open (0);
            break;
          }
        default: // In parent
          for (i = 0; i < high_priority_tasks; ++i)
            {
              high_priority_task[i].open (0);
            }
          break;
        }
    }
  else
    {
      for (i = 0; i < high_priority_tasks; ++i)
        {
          high_priority_task[i].open (0);
        }
      low_priority_task.open (0);

#if defined (ACE_HAS_THREADS)
     // Wait for all threads to exit.
     ACE_Thread_Manager::instance ()->wait ();
#endif /* ACE_HAS_THREADS */
    }

  // Display the time deltas.  They should be about a half second apart.
  if (child || ! use_fork)
    {
      for (i = 0; i < high_priority_tasks; ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "High priority task %u:\n", i + 1));
          high_priority_task[i].print_times ();
        }
    }

#else  /* ! ACE_HAS_THREADS && ACE_LACKS_FORK */
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_ERROR, "threads and fork not supported on this platform\n"));
#endif /* ! ACE_HAS_THREADS && ACE_LACKS_FORK */

  return 0;
}
