// $Id$
//
// ============================================================================
//
// = LIBRARY
//    (none)
//
// = FILENAME
//    context_switch_time.cpp
//
// = DESCRIPTION
//   Program that calculates context switch time between threads.
//   This test is based on the Task Context Switching measurement
//   approach described in:
//   Darren Cathey<br>
//   "RTOS Benchmarking -- All Things Considered . . ."<br>
//   <a href="http://www.realtime-info.be"><em>Real-Time Magazine</em></a>,
//      Second Quarter 1993,
//   <em>reprinted by <a href="http://www.wrs.com/artreqfm.html">Wind River
//                    Systems</a></em><p>
//   which in turn is based on Superconducting Super Collider (SSC)
//   Laboratory Ping Suspend/Resume Task and Suspend/Resume Task benchmarks.
//   It measures two different times:
//   1) The time to resume a blocked high priority task, which does
//      nothing other than block immediately.  A lower priority task
//      resumes the high priority task, so the elapsed time includes
//      two context switches, one task suspend, and one task resume.
//   2) The time to suspend and resume a low priority task that does
//      nothing.  There is no context switching.  This time is subtracted
//      from the one described in 1) above, and the result is divided by
//      two to yield the context switch time.
//
//   Notes:
//   On Solaris 2.5.1, it appears that the lowest context switching times,
//   at least on a single-CPU machine, are obtained _without_ creating new
//   LWPs for new threads (THR_NEW_LWP).  The -n option enables the use of
//   THR_NEW_LWP for testing.
//
// = CREATION DATE
//    17 January 1997
//
// = AUTHOR
//    David L. Levine
//
// ============================================================================

static const char usage [] = "[-? |\n"
                             "       [-c <repeat count, 0 means forever>]\n"
                             "       [-n to spawn a new LWP with each thread\n"
                             "[<iterations>]]";

#include "ace/Sched_Params.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"

#if defined (ACE_HAS_THREADS)

#if !defined (DEBUG)
# define DEBUG 0
#endif /* DEBUG */

static u_int LOW_PRIORITY;
static u_int HIGH_PRIORITY;

// Global test configuration parameters.
static ACE_UINT32 count = 1;
static ACE_UINT32 num_iterations = 1000;
static ACE_UINT32 new_lwp = 0;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Low_Priority_Null_Task
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class Low_Priority_Null_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Low_Priority_Null_Task ();
  virtual ~Low_Priority_Null_Task ();

  virtual int svc ();

  // Called by other task:  it returns when this task is ready to
  // continue.
  void ready () { initialized_.acquire (); }

  void done ();

  ACE_hthread_t thread_id () const { return thread_id_; }
private:
  ACE_hthread_t thread_id_;
  ACE_Semaphore initialized_;  // Blocks until thread_id_ is assigned.
  ACE_Semaphore blocked_semaphore_;

  // Force proper construction of independent instances.
  Low_Priority_Null_Task (const Low_Priority_Null_Task &);
  Low_Priority_Null_Task &operator= (const Low_Priority_Null_Task &);
};

inline
Low_Priority_Null_Task::Low_Priority_Null_Task() :
  ACE_Task<ACE_MT_SYNCH> (ACE_Thread_Manager::instance ()),
  initialized_ (0),  // initialize to locked, then unlock when ready
  blocked_semaphore_ (0)
{
#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Low_Priority_Null_Task ctor\n"));
#endif /* DEBUG */

  this->activate (THR_BOUND | THR_SCHED_FIFO | new_lwp, 1, 0, LOW_PRIORITY);

#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Low_Priority_Null_Task ctor, activated\n"));
#endif /* DEBUG */
}

Low_Priority_Null_Task::~Low_Priority_Null_Task()
{
}

int
Low_Priority_Null_Task::svc ()
{
#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Low_Priority_Null_Task::svc (), entering"));
#endif /* DEBUG */

  ACE_Thread_Manager::instance ()->thr_self (thread_id_);
  initialized_.release ();

#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "; thread ID is %u\n", thread_id_));
#endif /* DEBUG */

  // This task must never actually execute, so just have it block
  // on a semaphore forever . . .
  blocked_semaphore_.acquire ();

#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Low_Priority_Task::svc, finishing\n"));
#endif /* DEBUG */

  return 0;
}

void
Low_Priority_Null_Task::done ()
{
  blocked_semaphore_.release ();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Suspend_Resume_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class Suspend_Resume_Test : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Suspend_Resume_Test (const ACE_UINT32 iterations);
  virtual ~Suspend_Resume_Test ();

  virtual int svc ();

  ACE_hrtime_t elapsed_time () const { return elapsed_time_; }
private:
  const ACE_UINT32 iterations_;

  Low_Priority_Null_Task low_;

  ACE_High_Res_Timer timer_;

  ACE_hrtime_t elapsed_time_;

  // Force proper construction of independent instances.
  Suspend_Resume_Test ();
  Suspend_Resume_Test (const Suspend_Resume_Test &);
  Suspend_Resume_Test &operator= (const Suspend_Resume_Test &);
};

Suspend_Resume_Test::Suspend_Resume_Test (const ACE_UINT32 iterations) :
  ACE_Task<ACE_MT_SYNCH> (),
  iterations_ (iterations),
  low_ (),
  timer_ ()
{
#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Suspend_Resume_Test ctor\n"));
#endif /* DEBUG */

  this->activate (THR_BOUND | THR_SCHED_FIFO | new_lwp, 1, 0, HIGH_PRIORITY);
}

Suspend_Resume_Test::~Suspend_Resume_Test()
{
}

int
Suspend_Resume_Test::svc ()
{
#if DEBUG > 0
  ACE_hthread_t thread_id;
  ACE_Thread_Manager::instance ()->thr_self (thread_id);

  ACE_DEBUG ((LM_DEBUG, "Suspend_Resume_Test::svc (), thread ID is %d\n",
              thread_id));
#endif /* DEBUG */

  low_.ready ();

  // For information:  the cost of the just the loop itself below,
  // without the suspend and resume calls, on a 166 MHz Ultrasparc
  // is about 12.3 nanoseconds per iteration.

  timer_.start ();

  for (ACE_UINT32 i = 0; i < iterations_; ++i)
    {
#if DEBUG > 0
      if (i % (iterations_ >= 10  ?  iterations_ / 10  :  1) ==  0)
        ACE_DEBUG ((LM_DEBUG, "Suspend_Resume_Test::svc (), iteration %u\n",
                    i));
#endif /* DEBUG */

      if (ACE_OS::thr_suspend (low_.thread_id ()) != 0)
        {
          ACE_ERROR ((LM_ERROR, "%p\n", "thr_suspend"));
          low_.done ();
          return -1;
        }

      if (ACE_OS::thr_continue (low_.thread_id ()) != 0  &&
          errno != EINVAL)
        // EINVAL is OK:  it just means that the thread needs to be joined.
        {
          ACE_ERROR ((LM_ERROR, "%p\n", "thr_continue"));
          low_.done ();
          return -1;
        }
    }

  timer_.stop ();
  timer_.elapsed_microseconds (elapsed_time_);

  low_.done ();

#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Suspend_Resume_Test::svc, finishing\n"));
#endif /* DEBUG */

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class High_Priority_Simple_Task
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class High_Priority_Simple_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  High_Priority_Simple_Task ();
  virtual ~High_Priority_Simple_Task ();

  virtual int svc ();

  // called by other task:  it returns when this task is ready to
  // continue
  void ready () { initialized_.acquire (); }

  void done ();

  ACE_hthread_t thread_id () const { return thread_id_; }
  ACE_UINT32 iterations () const { return iterations_; }
private:
  ACE_hthread_t thread_id_;
  ACE_Semaphore initialized_;  // Block until thread_id_ is assigned.
  int terminate_;
  ACE_UINT32 iterations_;

  // Force proper construction of independent instances.
  High_Priority_Simple_Task (const High_Priority_Simple_Task &);
  High_Priority_Simple_Task &operator= (const High_Priority_Simple_Task &);
};

inline
High_Priority_Simple_Task::High_Priority_Simple_Task() :
  ACE_Task<ACE_MT_SYNCH> (ACE_Thread_Manager::instance ()),
  initialized_ (0),  // Initialize to locked, then unlock when ready.
  terminate_ (0),
  iterations_ (0)
{
#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "High_Priority_Simple_Task ctor\n"));
#endif /* DEBUG */

  this->activate (THR_BOUND | THR_SCHED_FIFO | new_lwp, 1, 0, HIGH_PRIORITY);

#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "High_Priority_Simple_Task ctor, activated\n"));
#endif /* DEBUG */
}

High_Priority_Simple_Task::~High_Priority_Simple_Task()
{
}

int
High_Priority_Simple_Task::svc ()
{
#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "High_Priority_Simple_Task::svc (), entering"));
#endif /* DEBUG */

  ACE_Thread_Manager::instance ()->thr_self (thread_id_);
  initialized_.release ();

#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "; thread ID is %u\n", thread_id_));
#endif /* DEBUG */

  for (ACE_UINT32 i = 0; ! terminate_; ++i)
    {
#if DEBUG > 0
      ACE_DEBUG ((LM_DEBUG, "High_Priority_Simple_Task::svc, suspend self ("
                            "%u)\n", thread_id_));
#endif /* DEBUG */

      ++iterations_;

      // immediately suspend self
      if (ACE_OS::thr_suspend (thread_id_) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "thr_suspend"), -1);
        }

#if DEBUG > 0
      ACE_DEBUG ((LM_DEBUG, "High_Priority_Simple_Task::svc, resumed (%u)\n",
                            thread_id_));
#endif /* DEBUG */
    }

#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "High_Priority_Simple_Task::svc, finishing\n"));
#endif /* DEBUG */

  return 0;
}

inline
void
High_Priority_Simple_Task::done ()
{
  terminate_ = 1;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Ping_Suspend_Resume_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class Ping_Suspend_Resume_Test : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Ping_Suspend_Resume_Test (const ACE_UINT32 iterations);
  virtual ~Ping_Suspend_Resume_Test ();

  virtual int svc ();

  ACE_hrtime_t elapsed_time () const { return elapsed_time_; }
private:
  const ACE_UINT32 iterations_;

  High_Priority_Simple_Task high_;

  ACE_High_Res_Timer timer_;

  ACE_hrtime_t elapsed_time_;

  // Force proper construction of independent instances.
  Ping_Suspend_Resume_Test ();
  Ping_Suspend_Resume_Test (const Ping_Suspend_Resume_Test &);
  Ping_Suspend_Resume_Test &operator= (const Ping_Suspend_Resume_Test &);
};

Ping_Suspend_Resume_Test::Ping_Suspend_Resume_Test (
  const ACE_UINT32 iterations)
:
  ACE_Task<ACE_MT_SYNCH> (),
  iterations_ (iterations),
  high_ (),
  timer_ ()
{
#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test ctor\n"));
#endif /* DEBUG */

  this->activate (THR_BOUND | THR_SCHED_FIFO | new_lwp, 1, 0, LOW_PRIORITY);
}

Ping_Suspend_Resume_Test::~Ping_Suspend_Resume_Test()
{
}

int
Ping_Suspend_Resume_Test::svc ()
{
#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test::svc (), entering"));

  ACE_hthread_t thread_id;
  ACE_Thread_Manager::instance ()->thr_self (thread_id);

  ACE_DEBUG ((LM_DEBUG, "; thread ID is %u\n", thread_id));
#endif /* DEBUG */

  high_.ready ();

#if DEBUG > 0
  int priority, high_priority;
  ACE_OS::thr_getprio (thread_id, priority);
  ACE_OS::thr_getprio (high_.thread_id (), high_priority);
  ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test::svc (), priority is %d, "
                        ", high thread priority is %d\n",
              priority, high_priority));
#endif /* DEBUG */

  // For information:  the cost of the just the loop itself below,
  // without the suspend and resume calls, on a 166 MHz Ultrasparc
  // is about 12.3 nanoseconds per iteration.

  timer_.start ();

  ACE_UINT32 i;

  for (i = 0; i < iterations_; ++i)
    {
#if DEBUG > 0
      if (i % (iterations_ >= 10  ?  iterations_ / 10  :  1) ==  0)
        {
          ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test::svc (), iteration "
                                "%d, continue high-priority thread %u\n",
                      i, high_.thread_id ()));
        }
#endif /* DEBUG */
      if (ACE_OS::thr_continue (high_.thread_id ()) != 0  &&
          errno != EINVAL)
        // EINVAL is OK:  it just means that the thread needs to be joined.
        {
          ACE_ERROR ((LM_ERROR, "%p\n", "thr_continue"));
          high_.done ();
          return -1;
        }
    }

  timer_.stop ();
  timer_.elapsed_microseconds (elapsed_time_);

  high_.done ();
#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test::svc: told high priority "
                        "task to terminate\n"));
#endif /* DEBUG */

  // Resume the thread until thr_continue fails, indicating that it has
  // finished.
  for (i = 0; i < 10000  &&  ! ACE_OS::thr_continue (high_.thread_id ());
       ++i) /* null */;

  // Don't count the one iteration that was used to allow the high-priority
  // thread to terminate.
  if (high_.iterations () < iterations_)
    ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test: high priority task "
                "executed only %u iterations!\n",
                high_.iterations ()));

#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test::svc, finishing\n"));
#endif /* DEBUG */

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Yield_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class Yield_Test : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Yield_Test (const ACE_UINT32 iterations);
  virtual ~Yield_Test ();

  virtual int svc ();

  ACE_hrtime_t elapsed_time () const { return elapsed_time_; }
private:
  const ACE_UINT32 iterations_;
  ACE_Barrier timer_barrier_;
  ACE_High_Res_Timer timer_;
  ACE_hrtime_t elapsed_time_;

  // Force proper construction of independent instances.
  Yield_Test ();
  Yield_Test (const Yield_Test &);
  Yield_Test &operator= (const Yield_Test &);
};

Yield_Test::Yield_Test (const ACE_UINT32 iterations) :
  ACE_Task<ACE_MT_SYNCH> (),
  iterations_ (iterations),
  timer_barrier_ (3),
  timer_ ()
{
#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Yield_Test ctor\n"));
#endif /* DEBUG */

  timer_.start ();

  this->activate (THR_BOUND | THR_SCHED_FIFO | new_lwp, 2, 0, LOW_PRIORITY);

  timer_barrier_.wait ();

  timer_.stop ();
  timer_.elapsed_microseconds (elapsed_time_);
}

Yield_Test::~Yield_Test()
{
}

int
Yield_Test::svc ()
{
#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Yield_Test::svc (), entering"));

  ACE_hthread_t thread_id;
  ACE_Thread_Manager::instance ()->thr_self (thread_id);

  int priority;
  ACE_OS::thr_getprio (thread_id, priority);

  ACE_DEBUG ((LM_DEBUG, "; thread ID is %u, priority is %u\n", thread_id,
              priority));
#endif /* DEBUG */

  for (ACE_UINT32 i = 0; i < iterations_; ++i)
    {
#if DEBUG > 0
      if (i % (iterations_ >= 10  ?  iterations_ / 10  :  1) ==  0)
        {
          ACE_DEBUG ((LM_DEBUG, "Yield_Test::svc () [%u], iteration %u\n",
                      thread_id, i));
        }
#endif /* DEBUG */

      ACE_OS::thr_yield ();
    }

  timer_barrier_.wait ();

#if DEBUG > 0
  ACE_DEBUG ((LM_DEBUG, "Yield_Test::svc, finishing\n"));
#endif /* DEBUG */

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function get_options
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static
u_int
get_options (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "c:n?");
  int opt;
  while ((opt = get_opt ()) != EOF) {
    switch (opt) {
    case 'c':
      if (ACE_OS::atoi (get_opt.optarg) >= 0)
        {
          count = ACE_OS::atoi (get_opt.optarg);
        }
      else
        {
          ACE_DEBUG ((LM_ERROR, "%n: count must be >= 0\n"));
          return 1;
        }
      break;
    case 'n':
      new_lwp = THR_NEW_LWP;
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
  case 1:
    if (ACE_OS::atoi (argv [get_opt.optind]) > 0)
      {
        num_iterations = ACE_OS::atoi (argv [get_opt.optind]);
      }
    else
      {
        ACE_DEBUG ((LM_ERROR, "%n: iterations must be > 0\n"));
        return 1;
      }
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
main (int argc, char *argv [])
{
  ACE_LOG_MSG->open (argv[0] > 0  ?  argv[0]  :  "context_switch_time");

  // Disable LM_DEBUG.
  ACE_Log_Msg::instance ()->priority_mask (ACE_LOG_MSG->priority_mask () ^
                                           LM_DEBUG);

  if (get_options (argc, argv)) ACE_OS::exit (-1);

  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
          ACE_Sched_Params::priority_min (ACE_SCHED_FIFO),
          ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_MAX, "context_switch_time: user is not superuser, "
                              "so remain in time-sharing class\n"));
        }
      else
        {
          ACE_OS::perror ("context_switch_time");
          ACE_OS::exit (-1);
        }
    }

  LOW_PRIORITY = ACE_THR_PRI_FIFO_DEF;
  HIGH_PRIORITY = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                   LOW_PRIORITY);
  ACE_DEBUG ((LM_INFO, "low priority: %d, high priority: %d\n",
              LOW_PRIORITY, HIGH_PRIORITY));

  // Set the priority of this thread so that it's higher than any of
  // the test threads.  That might help avoid problems when waiting on
  // those threads, below.  It also seems to help the times
  // significantly on LynxOS.
  ACE_hthread_t self;
  ACE_OS::thr_self (self);
  ACE_OS::thr_setprio (ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                        HIGH_PRIORITY));

  int forever = count == 0;

  while (forever  ||  count-- > 0)
    {
      // Check to see if thr_continue (), and therefore thr_suspend (),
      // probably, are supported.
      if (ACE_OS::thr_continue (self) == 0  ||
          errno != ENOTSUP)
        {
          // Run suspend/resume test first . . .
          Suspend_Resume_Test suspend_resume_test (num_iterations);
          // Wait for all tasks to exit.
          ACE_Thread_Manager::instance ()->wait ();

          // Then Ping Suspend/Resume test.
          Ping_Suspend_Resume_Test ping_suspend_resume_test (num_iterations);
          // Wait for all tasks to exit.
          ACE_Thread_Manager::instance ()->wait ();

          // NOTE:  the divisions by (ACE_UINT32) 1u below allow transparent
          // support of ACE_U_LongLongs.

          if (ping_suspend_resume_test.elapsed_time () >
               suspend_resume_test.elapsed_time ())
            {
              ACE_DEBUG ((LM_INFO, "context switch time is (%.3f - %.3f)/2 = "
                                   "%.3f microseconds\n",
                          (double) (ping_suspend_resume_test.elapsed_time () /
                                      (ACE_UINT32) 1u) /
                                    num_iterations,
                          (double) (suspend_resume_test.elapsed_time () /
                                      (ACE_UINT32) 1u) /
                                    num_iterations,
                          (double) ((ping_suspend_resume_test.elapsed_time () -
                                      suspend_resume_test.elapsed_time ()) /
                                      (ACE_UINT32) 1u) /
                                    num_iterations / 2));
            }
          else
            {
              ACE_DEBUG ((LM_INFO, "ping suspend/resume time of %.3f usec was "
                                   "less than suspend/resume time of %.3f\n",
                          (double) (ping_suspend_resume_test.elapsed_time () /
                                      (ACE_UINT32) 1u) /
                                    num_iterations,
                          (double) (suspend_resume_test.elapsed_time () /
                                      (ACE_UINT32) 1u) /
                                    num_iterations));
            }
        }

      // Then Yield test.
      Yield_Test yield_test (num_iterations);
      // Wait for all tasks to exit.
      ACE_Thread_Manager::instance ()->wait ();

      // Try _really_ hard not to use floating point.
      ACE_DEBUG ((LM_INFO, "context switch time from yield test is %u.%03u "
                           "microseconds\n",
                  (ACE_UINT32)
                    (yield_test.elapsed_time () / num_iterations / 2u),
                  (ACE_UINT32)
                    (yield_test.elapsed_time () % (num_iterations * 2u)) *
                      1000u / num_iterations / 2u));
    }

  return 0;
}
#else
int
main (int, char *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */


// EOF
