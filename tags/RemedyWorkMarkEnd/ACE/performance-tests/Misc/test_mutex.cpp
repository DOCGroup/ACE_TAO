// $Id$

// This test program illustrates the performance difference between
// three versions of wrappers for thread mutexes.  These three
// versions exercise various combinations of the following classes:
//
// Thread_Mutex --
//    This version is just like ACE_Thread_Mutex, which doesn't use
//    inheritance and dynamic binding.
//
// Mutex_Base --
//    This is an abstract base class that defines the
//    acquire()/release() interface.
//
// Thread_Mutex_Derived --
//    This derived from Mutex_Base and uses inheritance and
//    dynamic binding.
//
// The following are the results I got when running this on our
// SPARCstation 20 model 712:
//
// ./test_mutex 1000000
// iterations = 1000000
// Thread_Mutex
// real time = 1.727843 secs, user time = 1.729262 secs, system time = 0.000325 secs
// time per call = 1.747843 usecs
// Thread_Mutex_Derived
// real time = 1.730225 secs, user time = 1.724744 secs, system time = 0.000096 secs
// time per call = 1.730225 usecs
// Mutex_Base
// real time = 2.112831 secs, user time = 2.104245 secs, system time = 0.000095 secs
// time per call = 2.112831 usecs
//
// My conclusions are as follows:
//
// 1. If your C++ compiler optimizes calls to virtual functions that
//    are made through instances of derived classes, then the
//    performance of the Thread_Mutex and Thread_Mutex_Derived are
//    essentially identical.
//
// 2. The overhead from using virtual functions is approximately
//    20%.  Naturally, as the amount of contention goes up, the
//    relative overhead of the virtual function calls will decrease.
//
// Keep in mind, however, that using virtual functions to implement
// the Thread_Mutex will make it infeasible to put instances of
// Thread_Mutex into shared memory since the vptrs won't point to the
// correct vtables...

#include "ace/Log_Msg.h"
#include "ace/Profile_Timer.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_Thread.h"

#if defined (ACE_HAS_THREADS)

static const int DEFAULT_ITERATIONS = 100000000;

// A thread mutex that doesn't use virtual functions.
class Thread_Mutex
{
public:
  Thread_Mutex (void);
  ~Thread_Mutex (void);
  int acquire (void);
  int release (void);

private:
  ACE_mutex_t mutex_;
};

Thread_Mutex::Thread_Mutex (void)
{
  ACE_OS::mutex_init (&this->mutex_);
}

Thread_Mutex::~Thread_Mutex (void)
{
  ACE_OS::mutex_destroy (&this->mutex_);
}

inline int
Thread_Mutex::acquire (void)
{
  return ACE_OS::mutex_lock (&this->mutex_);
}

inline int
Thread_Mutex::release (void)
{
  return ACE_OS::mutex_unlock (&this->mutex_);
}

// Base class for mutex, declares pure virtual functions.
class Mutex_Base
{
public:
  virtual ~Mutex_Base (void);
  virtual int acquire (void) = 0;
  virtual int release (void) = 0;
};

Mutex_Base::~Mutex_Base (void)
{
}

// Subclass for threaded mutex, defines virtual functions.
class Thread_Mutex_Derived : public Mutex_Base
{
public:
  Thread_Mutex_Derived (void);
  virtual ~Thread_Mutex_Derived (void);
  virtual int acquire (void);
  virtual int release (void);

private:
  ACE_mutex_t mutex_;
};

Thread_Mutex_Derived::Thread_Mutex_Derived (void)
{
  ACE_OS::mutex_init (&this->mutex_);
}

Thread_Mutex_Derived::~Thread_Mutex_Derived (void)
{
  ACE_OS::mutex_destroy (&this->mutex_);
}

inline int
Thread_Mutex_Derived::acquire (void)
{
  return ACE_OS::mutex_lock (&this->mutex_);
}

inline int
Thread_Mutex_Derived::release (void)
{
  return ACE_OS::mutex_unlock (&this->mutex_);
}

static Thread_Mutex thread_mutex;
static Thread_Mutex_Derived thread_mutex_derived;
static Mutex_Base *mutex_base = &thread_mutex_derived;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Profile_Timer timer;
  int iterations = argc > 1 ? ACE_OS::atoi (argv[1]) : DEFAULT_ITERATIONS;
  int i;

  ACE_DEBUG ((LM_DEBUG, "iterations = %d\n", iterations));

  timer.start ();

  // Test the thread mutex (which doesn't use inheritance or dynamic
  // binding).

  for (i = 0; i < iterations; i++)
    {
      thread_mutex.acquire ();
      thread_mutex.release ();
    }

  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, "Thread_Mutex\n"));
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
              et.real_time, et.user_time, et.system_time));

  ACE_DEBUG ((LM_DEBUG, "time per call = %f usecs\n",
             (et.real_time / double (iterations)) * 1000000));

  // Test the thread mutex derived (which does use inheritance or
  // dynamic binding).  Note that we call this via an instance of the
  // derived class, so good C++ compilers should optimize the virtual
  // function calls in this case.

  timer.start ();

  for (i = 0; i < iterations; i++)
    {
      thread_mutex_derived.acquire ();
      thread_mutex_derived.release ();
    }

  timer.stop ();

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, "Thread_Mutex_Derived\n"));
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
             et.real_time, et.user_time, et.system_time));

  ACE_DEBUG ((LM_DEBUG, "time per call = %f usecs\n",
             (et.real_time / double (iterations)) * 1000000));

  // Test the thread mutex derived (which does use inheritance or
  // dynamic binding).  Note that we call this via a pointer to the
  // base class, which points to an instance of the derived class.
  // Thus, C++ compilers won't be able to optimize the virtual
  // function calls in this case.

  timer.start ();

  for (i = 0; i < iterations; i++)
    {
      mutex_base->acquire ();
      mutex_base->release ();
    }

  timer.stop ();

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, "Mutex_Base\n"));
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
             et.real_time, et.user_time, et.system_time));

  ACE_DEBUG ((LM_DEBUG, "time per call = %f usecs\n",
             (et.real_time / double (iterations)) * 1000000));
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
