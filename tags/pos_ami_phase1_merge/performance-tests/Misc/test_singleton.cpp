// $Id$

// This example illustrates the performance impact of using the
// Double-Checked Locking pattern compared with using the "standard"
// practice of acquiring and releasing a lock on every instance()
// call.  In addition, we compare the performance of using the
// ACE_Singleton (which encapsulates the Double-Check Locking pattern)
// vs. hand-coding the pattern.
//
// Here's the output from running this test on our SPARCstation 20, model 712s.
//
// ./test_singleton 1000000
// iterations = 1000000
// ACE_Singleton
// real time = 0.193731 secs, user time = 0.190416 secs, system time = 0.000549 secs
// time per call = 0.193731 usecs
// DC_Singleton
// real time = 0.176208 secs, user time = 0.176045 secs, system time = 0.000092 secs
// time per call = 0.176208 usecs
// Mutex_Singleton
// real time = 3.160998 secs, user time = 3.121434 secs, system time = 0.000109 secs
// time per call = 3.160998 usecs
//
// As you can see, both Double-Checked Locking implementations are about
// 15 times faster than the standard mutex version.  Moreover,
// this test is run with only a single thread, so there's no contention
// for the lock.  If there were multiple threads contending for the lock,
// the Mutex_Singleton performance would get increasing worse...

#include "ace/Profile_Timer.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/Log_Msg.h"

ACE_RCSID(Misc, test_singleton, "$Id$")

#if defined (ACE_HAS_THREADS)

static const int DEFAULT_ITERATIONS = 100000000;

class Mutex_Singleton
{
public:
  Mutex_Singleton (void) {}
  void svc (void) {}
  static Mutex_Singleton *instance (void);

private:
  static ACE_Thread_Mutex lock_;
  static Mutex_Singleton *instance_;
};

ACE_Thread_Mutex Mutex_Singleton::lock_;

Mutex_Singleton *Mutex_Singleton::instance_;

Mutex_Singleton *
Mutex_Singleton::instance (void)
{
  // Acquire the lock every time in.
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, Mutex_Singleton::lock_, 0);

  if (Mutex_Singleton::instance_ == 0)
    ACE_NEW_RETURN (Mutex_Singleton::instance_, Mutex_Singleton, 0);

  return Mutex_Singleton::instance_;
}

class DC_Singleton
{
public:
  DC_Singleton (void) {}
  void svc (void) {}
  static DC_Singleton *instance (void);

private:
  static ACE_Thread_Mutex lock_;
  static DC_Singleton *instance_;
};

ACE_Thread_Mutex DC_Singleton::lock_;

DC_Singleton *DC_Singleton::instance_;

DC_Singleton *
DC_Singleton::instance (void)
{
  if (DC_Singleton::instance_ == 0)
    {
      // Only lock if instance_ isn't 0.
      ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, DC_Singleton::lock_, 0);

      // Perform the Double-Check.
      if (DC_Singleton::instance_ == 0)
	ACE_NEW_RETURN (DC_Singleton::instance_, DC_Singleton, 0);
    }

  return DC_Singleton::instance_;
}

typedef ACE_Singleton <DC_Singleton, ACE_Thread_Mutex> My_Singleton;

int
main (int argc, char *argv[])
{
  ACE_Profile_Timer timer;
  int iterations = argc > 1 ? ACE_OS::atoi (argv[1]) : DEFAULT_ITERATIONS;
  int i;

  ACE_DEBUG ((LM_DEBUG, "iterations = %d\n", iterations));

  // Test the ACE_Singleton performance (which uses Double-Checked
  // Locking).

  timer.start ();

  for (i = 0; i < iterations; i++)
    My_Singleton::instance ()->svc ();

  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, "ACE_Singleton\n"));
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
	    et.real_time, et.user_time, et.system_time));

  ACE_DEBUG ((LM_DEBUG, "time per call = %f usecs\n",
	      (et.real_time / double (iterations)) * 1000000));

  // Test the hand-coded Singleton performance (which uses
  // Double-Checked Locking).

  timer.start ();

  for (i = 0; i < iterations; i++)
    DC_Singleton::instance ()->svc ();

  timer.stop ();

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, "DC_Singleton\n"));
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
	      et.real_time, et.user_time, et.system_time));

  ACE_DEBUG ((LM_DEBUG, "time per call = %f usecs\n",
	      (et.real_time / double (iterations)) * 1000000));

  // Test the Mutex_Singleton implementation (which does not use
  // Double-Checked Locking).

  timer.start ();

  for (i = 0; i < iterations; i++)
    Mutex_Singleton::instance ()->svc ();

  timer.stop ();

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, "Mutex_Singleton\n"));
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
	      et.real_time, et.user_time, et.system_time));

  ACE_DEBUG ((LM_DEBUG, "time per call = %f usecs\n",
	      (et.real_time / double (iterations)) * 1000000));

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<DC_Singleton, ACE_Thread_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<DC_Singleton, ACE_Thread_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#else
int
main (int, char *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
