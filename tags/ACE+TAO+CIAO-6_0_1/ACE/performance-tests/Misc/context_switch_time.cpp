// $Id$

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
//   The Suspend-Resume test is based on the Task Context Switching
//   measurement approach described in:
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
                             "       [-c <repeat counter, 0 means forever>]\n"
                             "       [-n to spawn a new LWP with each thread\n"
                             "[<iterations>]]";

#include "ace/OS_NS_stdio.h"
#include "ace/OS_main.h"
#include "ace/Task.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Thread_Semaphore.h"
#include "ace/Barrier.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_errno.h"

#if defined (ACE_HAS_THREADS)

#if !defined (ACE_DEBUG_CST)
# define ACE_DEBUG_CST 0
#endif /* ACE_DEBUG_CST */

static const u_int LOW_PRIORITY = ACE_THR_PRI_FIFO_DEF;
static u_int HIGH_PRIORITY;

// Global test configuration parameters.
static ACE_UINT32 counter = 1;
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
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Low_Priority_Null_Task ctor\n"));
#endif /* ACE_DEBUG_CST */

  if (this->activate (THR_BOUND | THR_DETACHED | THR_SCHED_FIFO | new_lwp,
                      1, 0, LOW_PRIORITY))
    ACE_OS::perror (ACE_TEXT("activate"));

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Low_Priority_Null_Task ctor, activated\n"));
#endif /* ACE_DEBUG_CST */
}

Low_Priority_Null_Task::~Low_Priority_Null_Task()
{
}

int
Low_Priority_Null_Task::svc ()
{
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Low_Priority_Null_Task::svc (), entering"));
#endif /* ACE_DEBUG_CST */

  ACE_Thread_Manager::instance ()->thr_self (thread_id_);
  initialized_.release ();

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "; thread ID is %u\n", thread_id_));
#endif /* ACE_DEBUG_CST */

  // This task must never actually execute, so just have it block
  // on a semaphore forever . . .
  blocked_semaphore_.acquire ();

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Low_Priority_Task::svc, finishing\n"));
#endif /* ACE_DEBUG_CST */

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
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Suspend_Resume_Test ctor\n"));
#endif /* ACE_DEBUG_CST */

  if (this->activate (THR_BOUND | THR_DETACHED | THR_SCHED_FIFO | new_lwp,
                  1, 0, HIGH_PRIORITY))
    ACE_OS::perror (ACE_TEXT("activate"));
}

Suspend_Resume_Test::~Suspend_Resume_Test()
{
}

int
Suspend_Resume_Test::svc ()
{
#if ACE_DEBUG_CST > 0
  ACE_hthread_t thread_id;
  ACE_Thread_Manager::instance ()->thr_self (thread_id);

  ACE_DEBUG ((LM_DEBUG, "Suspend_Resume_Test::svc (), thread ID is %d\n",
              thread_id));
#endif /* ACE_DEBUG_CST */

  low_.ready ();

  // For information:  the cost of the just the loop itself below,
  // without the suspend and resume calls, on a 166 MHz Ultrasparc
  // is about 12.3 nanoseconds per iteration.

  timer_.start ();

  for (ACE_UINT32 i = 0; i < iterations_; ++i)
    {
#if ACE_DEBUG_CST > 0
      if (i % (iterations_ >= 10  ?  iterations_ / 10  :  1) ==  0)
        ACE_DEBUG ((LM_DEBUG, "Suspend_Resume_Test::svc (), iteration %u\n",
                    i));
#endif /* ACE_DEBUG_CST */

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

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Suspend_Resume_Test::svc, finishing\n"));
#endif /* ACE_DEBUG_CST */

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
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "High_Priority_Simple_Task ctor\n"));
#endif /* ACE_DEBUG_CST */

  if (this->activate (THR_BOUND | THR_DETACHED | THR_SCHED_FIFO | new_lwp,
                  1, 0, HIGH_PRIORITY))
    ACE_OS::perror (ACE_TEXT("activate"));

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "High_Priority_Simple_Task ctor, activated\n"));
#endif /* ACE_DEBUG_CST */
}

High_Priority_Simple_Task::~High_Priority_Simple_Task()
{
}

int
High_Priority_Simple_Task::svc ()
{
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "High_Priority_Simple_Task::svc (), entering"));
#endif /* ACE_DEBUG_CST */

  ACE_Thread_Manager::instance ()->thr_self (thread_id_);
  initialized_.release ();

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "; thread ID is %u\n", thread_id_));
#endif /* ACE_DEBUG_CST */

  for (ACE_UINT32 i = 0; ! terminate_; ++i)
    {
#if ACE_DEBUG_CST > 0
      ACE_DEBUG ((LM_DEBUG, "High_Priority_Simple_Task::svc, suspend self ("
                            "%u)\n", thread_id_));
#endif /* ACE_DEBUG_CST */

      ++iterations_;

      // immediately suspend self
      if (ACE_OS::thr_suspend (thread_id_) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "thr_suspend"), -1);
        }

#if ACE_DEBUG_CST > 0
      ACE_DEBUG ((LM_DEBUG, "High_Priority_Simple_Task::svc, resumed (%u)\n",
                            thread_id_));
#endif /* ACE_DEBUG_CST */
    }

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "High_Priority_Simple_Task::svc, finishing\n"));
#endif /* ACE_DEBUG_CST */

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
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test ctor\n"));
#endif /* ACE_DEBUG_CST */

  if (this->activate (THR_BOUND | THR_DETACHED | THR_SCHED_FIFO | new_lwp,
                  1, 0, LOW_PRIORITY))
    ACE_OS::perror (ACE_TEXT("activate"));
}

Ping_Suspend_Resume_Test::~Ping_Suspend_Resume_Test()
{
}

int
Ping_Suspend_Resume_Test::svc ()
{
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test::svc (), entering"));

  ACE_hthread_t thread_id;
  ACE_Thread_Manager::instance ()->thr_self (thread_id);

  ACE_DEBUG ((LM_DEBUG, "; thread ID is %u\n", thread_id));
#endif /* ACE_DEBUG_CST */

  high_.ready ();

#if ACE_DEBUG_CST > 0
  int priority, high_priority;
  ACE_OS::thr_getprio (thread_id, priority);
  ACE_OS::thr_getprio (high_.thread_id (), high_priority);
  ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test::svc (), priority is %d, "
                        ", high thread priority is %d\n",
              priority, high_priority));
#endif /* ACE_DEBUG_CST */

  // For information:  the cost of the just the loop itself below,
  // without the suspend and resume calls, on a 166 MHz Ultrasparc
  // is about 12.3 nanoseconds per iteration.

  timer_.start ();

  ACE_UINT32 i;

  for (i = 0; i < iterations_; ++i)
    {
#if ACE_DEBUG_CST > 0
      if (i % (iterations_ >= 10  ?  iterations_ / 10  :  1) ==  0)
        {
          ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test::svc (), iteration "
                                "%d, continue high-priority thread %u\n",
                      i, high_.thread_id ()));
        }
#endif /* ACE_DEBUG_CST */
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
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test::svc: told high priority "
                        "task to terminate\n"));
#endif /* ACE_DEBUG_CST */

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

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Ping_Suspend_Resume_Test::svc, finishing\n"));
#endif /* ACE_DEBUG_CST */

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
#if defined (VXWORKS)
  ACE_Thread_Mutex mutex_;
  u_int started_;
  u_int stopped_;
#else  /* ! VXWORKS */
  ACE_Barrier timer_barrier_;
#endif /* ! VXWORKS */
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
#if defined (VXWORKS)
  mutex_ (),
  started_ (0),
  stopped_ (0),
#else  /* ! VXWORKS */
  timer_barrier_ (3),
#endif /* ! VXWORKS */
  timer_ ()
{
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Yield_Test ctor\n"));
#endif /* ACE_DEBUG_CST */

#if !defined (VXWORKS)
  timer_.start ();
#endif /* ! VXWORKS */

  if (this->activate (THR_BOUND | THR_DETACHED | THR_SCHED_FIFO | new_lwp,
                  2, 0, LOW_PRIORITY))
    ACE_OS::perror (ACE_TEXT("activate"));

#if !defined (VXWORKS)
  timer_barrier_.wait ();
  timer_.stop ();
#endif /* ! VXWORKS */

  timer_.elapsed_microseconds (elapsed_time_);
}

Yield_Test::~Yield_Test()
{
}

int
Yield_Test::svc ()
{
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Yield_Test::svc (), entering"));

  ACE_hthread_t thread_id;
  ACE_Thread_Manager::instance ()->thr_self (thread_id);

  int priority;
  ACE_OS::thr_getprio (thread_id, priority);

  ACE_DEBUG ((LM_DEBUG, "; thread ID is %u, priority is %u\n", thread_id,
              priority));
#endif /* ACE_DEBUG_CST */

#if defined (VXWORKS)
  // Start the timer, if it hasn't already been started.
  if (! started_)
    {
      // Double-check.
      ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, mutex_, -1);

      if (! started_)
        {
          started_ = 1;
          timer_.start ();
        }
    }
#endif /* VXWORKS */

  for (ACE_UINT32 i = 0; i < iterations_; ++i)
    {
#if ACE_DEBUG_CST > 0
      if (i % (iterations_ >= 10  ?  iterations_ / 10  :  1) ==  0)
        {
          ACE_DEBUG ((LM_DEBUG, "Yield_Test::svc () [%u], iteration %u\n",
                      thread_id, i));
        }
#endif /* ACE_DEBUG_CST */

      ACE_OS::thr_yield ();
    }

#if defined (VXWORKS)
  // Stop the timer, if it hasn't already been started.
  if (! stopped_)
    {
      // Maybe it would be better to read the clock before grabbing
      // the mutex.  Then, only apply the clock reading below, instead
      // of reading the clock after grabbing the mutex.

      // Double-check.
      ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, mutex_, -1);

      if (! stopped_)
        {
          stopped_ = 1;
          timer_.stop ();
          timer_.elapsed_time (elapsed_time_); /* nanoseconds */
        }
    }
#else  /* ! VXWORKS */
  timer_barrier_.wait ();
#endif /* ! VXWORKS */

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Yield_Test::svc, finishing\n"));
#endif /* ACE_DEBUG_CST */

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Mutex_Acquire_Release_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class Mutex_Acquire_Release_Test
{
public:
  Mutex_Acquire_Release_Test (const ACE_UINT32 iterations);
  virtual ~Mutex_Acquire_Release_Test ();

  virtual int svc ();

  ACE_hrtime_t elapsed_time () const { return elapsed_time_; }
private:
  ACE_Thread_Mutex mutex_;
  // Mutex used for acquire/release time measurement.

  ACE_Thread_Semaphore sem_;
  // Semaphore used for acquire/release time measurement.

  const ACE_UINT32 iterations_;

  ACE_High_Res_Timer timer_;

  ACE_hrtime_t elapsed_time_;

  // Force proper construction of independent instances.
  Mutex_Acquire_Release_Test ();
  Mutex_Acquire_Release_Test (const Mutex_Acquire_Release_Test &);
  Mutex_Acquire_Release_Test &operator= (const Mutex_Acquire_Release_Test &);
};

Mutex_Acquire_Release_Test::Mutex_Acquire_Release_Test (
    const ACE_UINT32 iterations) :
  mutex_ (),
  sem_ (),
  iterations_ (iterations),
  timer_ ()
{
}

Mutex_Acquire_Release_Test::~Mutex_Acquire_Release_Test()
{
}

int
Mutex_Acquire_Release_Test::svc ()
{
#if ACE_DEBUG_CST > 0
  ACE_hthread_t thread_id;
  ACE_Thread_Manager::instance ()->thr_self (thread_id);

  ACE_DEBUG ((LM_DEBUG,
              "Mutex_Acquire_Release_Test::svc (), thread ID is %d\n",
              thread_id));
#endif /* ACE_DEBUG_CST */

  timer_.start ();

  for (ACE_UINT32 i = 0; i < iterations_; ++i)
    {
      // Block on the mutex.
      ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, mutex_, -1);

      // Release the mutex so that the low priority thread can
      // proceed.  The ACE_GUARD_RETURN macro implicity releases the
      // mutex.
    }

  timer_.stop ();
  timer_.elapsed_time (elapsed_time_); /* nanoseconds */

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Mutex_Acquire_Release_Test::svc, finishing\n"));
#endif /* ACE_DEBUG_CST */

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class High_Priority_Synchronized_Task
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class High_Priority_Synchronized_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  High_Priority_Synchronized_Task (ACE_Thread_Semaphore &sem,
                                   ACE_Thread_Mutex &mutex,
                                   ACE_High_Res_Timer &timer);
  virtual ~High_Priority_Synchronized_Task ();

  virtual int svc ();

  void ready () { initialized_.acquire (); }
  // Called by other task:  it returns when this task is ready to
  // continue

  void done ();

  ACE_UINT32 average_context_switch_time () const;

  ACE_hthread_t thread_id () const { return thread_id_; }
  ACE_UINT32 iterations () const { return iterations_; }
private:
  ACE_hthread_t thread_id_;
  ACE_Semaphore initialized_;  // Block until thread_id_ is assigned.
  int terminate_;
  ACE_UINT32 iterations_;

  ACE_Thread_Semaphore &sem_;
  // Semaphore used to resume the task.

  ACE_Thread_Mutex &mutex_;
  // Mutex used to block the task.

  ACE_High_Res_Timer &timer_;
  // Clock shared between low and high priority tasks.

  ACE_hrtime_t total_time_;
  // Running total context switch time, nsec.

  // Force proper construction of independent instances.
  High_Priority_Synchronized_Task ();
  High_Priority_Synchronized_Task (const High_Priority_Synchronized_Task &);
  High_Priority_Synchronized_Task &
    operator= (const High_Priority_Synchronized_Task &);
};

High_Priority_Synchronized_Task::High_Priority_Synchronized_Task (
    ACE_Thread_Semaphore &sem,
    ACE_Thread_Mutex &mutex,
    ACE_High_Res_Timer &timer) :
  ACE_Task<ACE_MT_SYNCH> (ACE_Thread_Manager::instance ()),
  initialized_ (0),  // Initialize to locked, then unlock when ready.
  terminate_ (0),
  iterations_ (0),
  sem_ (sem),
  mutex_ (mutex),
  timer_ (timer),
  total_time_ (0)
{
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "High_Priority_Synchronized_Task ctor\n"));
#endif /* ACE_DEBUG_CST */

  if (this->activate (THR_BOUND | THR_DETACHED | THR_SCHED_FIFO | new_lwp,
                  1, 0, HIGH_PRIORITY))
    ACE_OS::perror (ACE_TEXT("activate"));

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "High_Priority_Synchronized_Task ctor, activated\n"));
#endif /* ACE_DEBUG_CST */
}

High_Priority_Synchronized_Task::~High_Priority_Synchronized_Task()
{
}

int
High_Priority_Synchronized_Task::svc ()
{
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "High_Priority_Synchronized_Task::svc (), entering"));
#endif /* ACE_DEBUG_CST */

  ACE_Thread_Manager::instance ()->thr_self (thread_id_);

  ACE_UINT32 mutex_acquire_release_time = 0;
  {
    Mutex_Acquire_Release_Test mutex_acquire_release_test (num_iterations);
    mutex_acquire_release_test.svc ();
    mutex_acquire_release_time =
      static_cast<ACE_UINT32> (mutex_acquire_release_test.elapsed_time () /
                       num_iterations);
#if ACE_DEBUG_CST > 0
    ACE_DEBUG ((LM_DEBUG, "mutex_acquire_release: %u nsec\n",
                mutex_acquire_release_time));
#endif /* ACE_DEBUG_CST */
  }

  initialized_.release ();

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "; thread ID is %u\n", thread_id_));
#endif /* ACE_DEBUG_CST */

  for (ACE_UINT32 i = 0; ! terminate_; ++i)
    {
#if ACE_DEBUG_CST > 0
      ACE_DEBUG ((LM_DEBUG,
                  "High_Priority_Synchronized_Task::svc, wait on sem ("
                  "%u)\n", thread_id_));
#endif /* ACE_DEBUG_CST */

      if (sem_.acquire () != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "sem_.acquire"), -1);
        }

      {
        // Block on the mutex.
        ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, mutex_, -1);

        timer_.stop ();

        ++iterations_;

        ACE_hrtime_t nsec;
        timer_.elapsed_time (nsec);
        const ACE_UINT32 context_switch_time =
          ACE_U64_TO_U32 (nsec) >= mutex_acquire_release_time  ?
            ACE_U64_TO_U32 (nsec) - mutex_acquire_release_time  :  0;

        total_time_ += context_switch_time;

        // Release the mutex so that the low priority thread can
        // proceed.  The ACE_GUARD_RETURN macro implicity releases the
        // mutex.
      }

#if ACE_DEBUG_CST > 0
      ACE_DEBUG ((LM_DEBUG,
                  "High_Priority_Synchronized_Task::svc, resumed (%u)\n",
                  thread_id_));
#endif /* ACE_DEBUG_CST */
    }

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "High_Priority_Synchronized_Task::svc, finishing\n"));
#endif /* ACE_DEBUG_CST */

  return 0;
}

inline
void
High_Priority_Synchronized_Task::done ()
{
  terminate_ = 1;
}

ACE_UINT32
High_Priority_Synchronized_Task:: average_context_switch_time () const
{
  return iterations_ > 0  ?  static_cast<ACE_UINT32> (total_time_ / iterations_)
                          : 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Synchronized_Suspend_Resume_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class Synchronized_Suspend_Resume_Test : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Synchronized_Suspend_Resume_Test (const ACE_UINT32 iterations);
  virtual ~Synchronized_Suspend_Resume_Test ();

  virtual int svc ();

  ACE_UINT32 average_context_switch_time ();

  ACE_hrtime_t elapsed_time () const { return elapsed_time_; }
private:
  const ACE_UINT32 iterations_;

  ACE_Thread_Semaphore sem_;
  // Used by the low priority thread to resume the high priority thread.

  ACE_Thread_Mutex mutex_;
  // Used by the low priority thread to block the high priority thread.

  ACE_High_Res_Timer timer_;
  // Clock shared between low and high priority tasks.

  High_Priority_Synchronized_Task high_;
  // The high priority task.

  ACE_hrtime_t elapsed_time_;

  ACE_UINT32 mutex_acquire_release_time_;

  // Force proper construction of independent instances.
  Synchronized_Suspend_Resume_Test ();
  Synchronized_Suspend_Resume_Test (const Synchronized_Suspend_Resume_Test &);
  Synchronized_Suspend_Resume_Test &
    operator= (const Synchronized_Suspend_Resume_Test &);
};

Synchronized_Suspend_Resume_Test::Synchronized_Suspend_Resume_Test (
  const ACE_UINT32 iterations)
:
  ACE_Task<ACE_MT_SYNCH> (),
  iterations_ (iterations),
  sem_ (0),
  mutex_ (),
  timer_ (),
  high_ (sem_, mutex_, timer_),
  mutex_acquire_release_time_ (0)
{
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Synchronized_Suspend_Resume_Test ctor\n"));
#endif /* ACE_DEBUG_CST */

  if (this->activate (THR_BOUND | THR_DETACHED | THR_SCHED_FIFO | new_lwp,
                  1, 0, LOW_PRIORITY))
    ACE_OS::perror (ACE_TEXT("activate"));
}

Synchronized_Suspend_Resume_Test::~Synchronized_Suspend_Resume_Test()
{
}

int
Synchronized_Suspend_Resume_Test::svc ()
{
#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Synchronized_Suspend_Resume_Test::svc (), entering"));

  ACE_hthread_t thread_id;
  ACE_Thread_Manager::instance ()->thr_self (thread_id);

  ACE_DEBUG ((LM_DEBUG, "; thread ID is %u\n", thread_id));
#endif /* ACE_DEBUG_CST */

  {
    Mutex_Acquire_Release_Test mutex_acquire_release_test (num_iterations);
    mutex_acquire_release_test.svc ();
    mutex_acquire_release_time_ =
      static_cast<ACE_UINT32> (mutex_acquire_release_test.elapsed_time () /
                       num_iterations);
#if ACE_DEBUG_CST > 0
    ACE_DEBUG ((LM_DEBUG, "mutex_acquire_release: %u nsec\n",
                mutex_acquire_release_time_));
#endif /* ACE_DEBUG_CST */
  }

  high_.ready ();

#if ACE_DEBUG_CST > 0
  int priority, high_priority;
  ACE_OS::thr_getprio (thread_id, priority);
  ACE_OS::thr_getprio (high_.thread_id (), high_priority);
  ACE_DEBUG ((LM_DEBUG,
              "Synchronized_Suspend_Resume_Test::svc (), priority is %d, "
              ", high thread priority is %d\n",
              priority, high_priority));
#endif /* ACE_DEBUG_CST */

  // For information:  the cost of the just the loop itself below,
  // without the suspend and resume calls, on a 166 MHz Ultrasparc
  // is about 12.3 nanoseconds per iteration.

  ACE_UINT32 i;

  for (i = 0; i < iterations_; ++i)
    {
#if ACE_DEBUG_CST > 0
      if (i % (iterations_ >= 10  ?  iterations_ / 10  :  1) ==  0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Synchronized_Suspend_Resume_Test::svc (), iteration "
                      "%d, continue high-priority thread %u\n",
                      i, high_.thread_id ()));
        }
#endif /* ACE_DEBUG_CST */

      {
        // Acquire the mutex so that the high priority thread will
        // block after we signal it via the condition variable.
        ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, mutex_, -1);

        // Release the semaphore so that the high priority thread can
        // proceed.
        if (sem_.release () != 0)
          ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "sem_.release"), -1);

        timer_.start ();

        // Release the mutex so that the high priority thread can
        // proceed.  The ACE_GUARD_RETURN macro implicity releases
        // the mutex.
      }
    }

  high_.done ();

  // The high priority thread will be block on the semaphore, so
  // release it.
  if (sem_.release () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "sem_.release"), -1);

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG,
              "Synchronized_Suspend_Resume_Test::svc: told high priority "
              "task to terminate\n"));
#endif /* ACE_DEBUG_CST */

  // Resume the thread until thr_continue fails, indicating that it has
  // finished.
  for (i = 0; i < 10000  &&  ! ACE_OS::thr_continue (high_.thread_id ());
       ++i) /* null */;

#if ACE_DEBUG_CST > 0
  ACE_DEBUG ((LM_DEBUG, "Synchronized_Suspend_Resume_Test::svc, finishing\n"));
#endif /* ACE_DEBUG_CST */

  return 0;
}

ACE_UINT32
Synchronized_Suspend_Resume_Test::average_context_switch_time ()
{
  return high_.average_context_switch_time ();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function get_options
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static
u_int
get_options (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("c:n?"));
  int opt;
  while ((opt = get_opt ()) != EOF) {
    switch (opt) {
    case 'c':
      if (ACE_OS::atoi (get_opt.opt_arg ()) >= 0)
        {
          counter = ACE_OS::atoi (get_opt.opt_arg ());
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
      ACE_DEBUG ((LM_ERROR, "usage: %n %s\n", usage));
      ACE_OS::exit (1);
      /* NOTREACHED */
      break;
    default:
      ACE_DEBUG ((LM_ERROR, "%n: unknown arg, %c\n", opt));
      ACE_DEBUG ((LM_ERROR, "usage: %n %s\n", usage));
      return 1;
    }
  }

  switch (argc - get_opt.opt_ind ()) {
  case 0:
    // use default number of iterations
    break;
  case 1:
    if (ACE_OS::atoi (argv [get_opt.opt_ind ()]) > 0)
      num_iterations = ACE_OS::atoi (argv [get_opt.opt_ind ()]);
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
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  ACE_LOG_MSG->open (argv[0] > 0  ?  argv[0]  :  ACE_TEXT("context_switch_time"));

  if (get_options (argc, argv))
    ACE_OS::exit (-1);

  // Disable LM_DEBUG.
  ACE_Log_Msg::instance ()->priority_mask (ACE_LOG_MSG->priority_mask () ^
                                           LM_DEBUG);

#if defined (ACE_HAS_PENTIUM)  &&  \
    !defined (ACE_HAS_HI_RES_TIMER)  &&  !defined (ACE_WIN32)
  // Just to verify that ACE_High_Res_Timer::global_scale_factor ()
  // correctly determines the clock speed.
  ACE_DEBUG ((LM_INFO, "clock speed: %u MHz\n",
              ACE_High_Res_Timer::global_scale_factor ()));
#endif /* ACE_HAS_PENTIUM && ! ACE_HAS_HI_RES_TIMER && ! ACE_WIN32 */

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
          ACE_OS::perror (ACE_TEXT("context_switch_time"));
          ACE_OS::exit (-1);
        }
    }

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

  bool forever = counter == 0;

  ACE_Stats context_switch_test_stats;
  ACE_Stats yield_test_stats;
  ACE_Stats synchronized_suspend_resume_test_stats;

  int suspend_resume_supported = 0;
  // Check to see if thr_continue (), and therefore thr_suspend (),
  // probably, are supported.
  if (ACE_OS::thr_continue (self) == 0  ||  errno != ENOTSUP)
    suspend_resume_supported = 1;

 while (forever  ||  counter-- > 0)
    {
      if (suspend_resume_supported)
        {
          // Run suspend/resume test first . . .
          Suspend_Resume_Test suspend_resume_test (num_iterations);
          // Wait for all tasks to exit.
          ACE_Thread_Manager::instance ()->wait ();

          // Then Ping Suspend/Resume test.
          Ping_Suspend_Resume_Test ping_suspend_resume_test (num_iterations);
          // Wait for all tasks to exit.
          ACE_Thread_Manager::instance ()->wait ();

          if (ping_suspend_resume_test.elapsed_time () >
               suspend_resume_test.elapsed_time ())
            {
              context_switch_test_stats.
                sample (ACE_U64_TO_U32 (
                  ping_suspend_resume_test.elapsed_time () -
                  suspend_resume_test.elapsed_time ()));

              ACE_DEBUG ((LM_INFO, "context switch time is (%.3f - %.3f)/2 = "
                                   "%.3f microseconds\n",
                          (double) ACE_UINT64_DBLCAST_ADAPTER (
                            ping_suspend_resume_test.elapsed_time ()) /
                              num_iterations,
                          (double) ACE_UINT64_DBLCAST_ADAPTER (
                            suspend_resume_test.elapsed_time ()) /
                              num_iterations,
                          (double) ACE_UINT64_DBLCAST_ADAPTER (
                            ping_suspend_resume_test.elapsed_time () -
                            suspend_resume_test.elapsed_time ()) /
                              num_iterations / 2u));
            }
          else
            {
              ACE_DEBUG ((LM_INFO, "ping suspend/resume time of %u usec was "
                                   "less than suspend/resume time of %u\n",
                                   ping_suspend_resume_test.elapsed_time () /
                                      num_iterations,
                                   suspend_resume_test.elapsed_time () /
                                      num_iterations));
            }
        }

      // Then Yield test.
      Yield_Test yield_test (num_iterations);
      // Wait for all tasks to exit.
      ACE_Thread_Manager::instance ()->wait ();

      yield_test_stats.sample (ACE_U64_TO_U32 (yield_test.elapsed_time ()));

      // Try _really_ hard not to use floating point.
      ACE_DEBUG ((LM_INFO, "context switch time from yield test is %u.%03u "
                           "microseconds\n",
                  (ACE_UINT32)
                    (yield_test.elapsed_time () / num_iterations / 2u),
                  (ACE_UINT32)
                    (yield_test.elapsed_time () % (num_iterations * 2u)) *
                      1000u / num_iterations / 2u));

      Synchronized_Suspend_Resume_Test
        synchronized_suspend_resume_test (num_iterations);
      // Wait for all tasks to exit.
      ACE_Thread_Manager::instance ()->wait ();

      synchronized_suspend_resume_test_stats.sample (
        synchronized_suspend_resume_test.average_context_switch_time ());

      ACE_DEBUG ((LM_INFO, "context switch time from synch susp/resume test "
                           "is %u.%03u microseconds\n",
                  synchronized_suspend_resume_test.
                    average_context_switch_time () / 1000u,
                  synchronized_suspend_resume_test.
                    average_context_switch_time () % 1000u));

      // Give, e.g., Draft 4 Posix platforms a chance to cleanup threads.
      const ACE_Time_Value half_sec (0L, 500000L);
      ACE_OS::sleep (half_sec);
    }

  if (suspend_resume_supported)
    {
      ACE_OS::printf ("suspend-resume test: ");
      context_switch_test_stats.print_summary (3, num_iterations * 2u);
    }

  ACE_OS::printf ("\nyield_test: ");
  yield_test_stats.print_summary (3, num_iterations * 2u);

  ACE_OS::printf ("\nsynchronized suspend-resume test: ");
  synchronized_suspend_resume_test_stats.print_summary (3,
                                                        1000u /* nsec/usec */);

  return 0;
}
#else
int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */


// EOF
