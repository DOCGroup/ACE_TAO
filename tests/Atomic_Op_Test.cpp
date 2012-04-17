
//=============================================================================
/**
 *  @file    Atomic_Op_Test.cpp
 *
 *  $Id$
 *
 *    This is a simple test of the Atomic Operations Class in ACE.
 *    On platforms like Win32, ACE uses template specialization to
 *    use native implementations provided by the OS to accelarate
 *    these operations.
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"

#include "ace/Atomic_Op.h"
#include "ace/Synch_Traits.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Barrier.h"
#include "ace/Task.h"


/*
 * Exchange_Test tests the exchange() operation in ACE_Atomic_Op. It runs
 * a number of threads and each tries to "claim" the op_ by exchanging it
 * with '1'. Only one thread should do this. The claimed_ member counts
 * the number of threads that actually do claim it.
 */
template <typename T>
class Exchange_Tester : public ACE_Task<ACE_NULL_SYNCH>
{
public:
  Exchange_Tester (unsigned int thr_count);
  int result (void) const;

private:
  Exchange_Tester () {}
  int svc (void);

  ACE_Barrier barrier_;
  ACE_Atomic_Op<ACE_SYNCH_MUTEX, T> op_;
  ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> claimed_;
};

template <typename T>
Exchange_Tester<T>::Exchange_Tester (unsigned int thr_count)
  : barrier_(thr_count), op_ (0), claimed_ (0)
{
  this->activate (THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED,
                  (int)thr_count);
}

template <typename T>
int
Exchange_Tester<T>::result (void) const
{
  return this->claimed_ == 1 ? 0 : 1;
}

template <typename T>
int
Exchange_Tester<T>::svc (void)
{
  this->barrier_.wait ();     // Want all threads to try to claim "at once"
  bool claimed = this->op_.exchange (1) == 0;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Thread %t %s claim.\n"),
              claimed ? ACE_TEXT ("DID") : ACE_TEXT ("DID NOT")));
  if (claimed)
    ++this->claimed_;
  return 0;
}


template <typename TYPE, typename dummy>
int test (const ACE_TCHAR* type, int iterations)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Measuring %d iterations for %s\n"), iterations, type));

  int retval = 0;
  ACE_Atomic_Op <ACE_SYNCH_MUTEX, TYPE> foo (5);

  ACE_TEST_ASSERT (foo == 5);

  TYPE result = ++foo;
  ACE_TEST_ASSERT (foo == 6);
  ACE_TEST_ASSERT (result == 6);

  result = --foo;
  ACE_TEST_ASSERT (foo == 5);
  ACE_TEST_ASSERT (result == 5);

  result = foo++;
  ACE_TEST_ASSERT (foo == 6);
  ACE_TEST_ASSERT (result == 5);

  result = foo--;
  ACE_TEST_ASSERT (foo == 5);
  ACE_TEST_ASSERT (result == 6);

  result = foo += 10;
  ACE_TEST_ASSERT (foo == 15);
  ACE_TEST_ASSERT (result == 15);

  result = foo -= 10;
  ACE_TEST_ASSERT (foo == 5);
  ACE_TEST_ASSERT (result == 5);

  foo = 7;
  ACE_TEST_ASSERT (foo == 7);

  ACE_Atomic_Op <ACE_SYNCH_MUTEX, TYPE> foo2 (5);
  foo2 = foo;
  ACE_TEST_ASSERT (foo == 7);
  ACE_TEST_ASSERT (foo2 == 7);

  ACE_UINT64 usec;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> assignment %D\n"), type));
  ACE_Time_Value diff = ACE_OS::gettimeofday ();
  int i;
  for (i = 0; i < iterations; ++i)
    {
      foo = 1;
      foo = 2;
      foo = 3;
      foo = 4;
    }
  diff = ACE_OS::gettimeofday () - diff;
  diff.to_usec (usec);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <%s> assignment %D, took %Q\n"), type, usec));

  foo = 0;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> prefix increment %D\n"), type));
  diff = ACE_OS::gettimeofday ();
  for (i = 0; i < iterations; ++i)
    {
      ++foo;
      ++foo;
      ++foo;
      ++foo;
    }
  diff = ACE_OS::gettimeofday () - diff;
  diff.to_usec (usec);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <%s> prefix increment %D, took %Q\n"), type, usec));

  if (foo != iterations * 4)
    {
      ++retval;
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Prefix increment failed\n")));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> prefix decrement %D\n"), type));
  diff = ACE_OS::gettimeofday ();
  for (i = 0; i < iterations; ++i)
    {
      foo--;
      foo--;
      foo--;
      foo--;
    }
  diff = ACE_OS::gettimeofday () - diff;
  diff.to_usec (usec);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <%s> prefix decrement %D, took %Q\n"), type, usec));

  if (foo != 0)
    {
      ++retval;
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Prefix decrement failed\n")));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> postfix increment %D\n"), type));
  diff = ACE_OS::gettimeofday ();
  for (i = 0; i < iterations; ++i)
    {
      foo++;
      foo++;
      foo++;
      foo++;
    }
  diff = ACE_OS::gettimeofday () - diff;
  diff.to_usec (usec);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <%s> postfix increment %D, took %Q\n"), type, usec));

  if (foo != iterations * 4)
    {
      ++retval;
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Postfix increment failed\n")));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> postfix decrement %D\n"), type));
  diff = ACE_OS::gettimeofday ();
  for (i = 0; i < iterations; ++i)
    {
      --foo;
      --foo;
      --foo;
      --foo;
    }
  diff = ACE_OS::gettimeofday () - diff;
  diff.to_usec (usec);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <%s> postfix decrement %D, took %Q\n"), type, usec));

  if (foo != 0)
    {
      ++retval;
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Postfix decrement failed\n")));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> addition %D\n"), type));
  diff = ACE_OS::gettimeofday ();
  for (i = 0; i < iterations; ++i)
    {
      foo += 5;
      foo += 5;
      foo += 5;
      foo += 5;
    }
  diff = ACE_OS::gettimeofday () - diff;
  diff.to_usec (usec);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <%s> addition %D, took %Q\n"), type, usec));

  if (foo != iterations * 4 * 5)
    {
      ++retval;
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Addition failed\n")));
     }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> subtraction %D\n"), type));
  diff = ACE_OS::gettimeofday ();
  for (i = 0; i < iterations; ++i)
    {
      foo -= 5;
      foo -= 5;
      foo -= 5;
      foo -= 5;
    }
  diff = ACE_OS::gettimeofday () - diff;
  diff.to_usec (usec);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <%s> subtraction %D, took %Q\n"), type, usec));

  if (foo != 0)
    {
      ++retval;
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Subtraction failed\n")));
    }

  return retval;
}

template <typename TYPE>
int test (const ACE_TCHAR* type, int iterations)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Measuring %d iterations for %s\n"), iterations, type));

  ACE_UINT64 usec;
  int retval = 0;
  ACE_Atomic_Op <ACE_SYNCH_MUTEX, TYPE> foo (true);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> assignment %D\n"), type));
  ACE_Time_Value diff = ACE_OS::gettimeofday ();
  int i;
  for (i = 0; i < iterations; ++i)
    {
      foo = true;
      foo = true;
      foo = true;
      foo = true;
    }
  diff = ACE_OS::gettimeofday () - diff;
  diff.to_usec (usec);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <%s> assignment %D, took %Q\n"), type, usec));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> comparison %D\n"), type));
  diff = ACE_OS::gettimeofday ();
  for (i = 0; i < iterations; ++i)
    {
      if (foo != true) ++retval;
      if (foo == false) ++retval;
      if (foo != true) ++retval;
      if (foo == false) ++retval;
    }
  diff = ACE_OS::gettimeofday () - diff;
  diff.to_usec (usec);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <%s> comparison %D, took %Q\n"), type, usec));

  return retval;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Atomic_Op_Test"));

  // Some platforms w/o builtin atomic ops time out the regression tests,
  // so run fewer iterations there.
#if defined (ACE_HAS_BUILTIN_ATOMIC_OP)
  const int ITERATIONS = 1000000;
#else
  const int ITERATIONS = 100000;
#endif /* ACE_HAS_BUILTIN_ATOMIC_OP */

  int retval = 0;

  retval += test <int, int> (ACE_TEXT("int"), ITERATIONS);
  retval += test <long, int> (ACE_TEXT("long"), ITERATIONS);
  retval += test <unsigned int, int> (ACE_TEXT("unsigned int"), ITERATIONS);
  retval += test <unsigned long, int> (ACE_TEXT("unsigned long"), ITERATIONS);
  retval += test <short, int> (ACE_TEXT("short"), ITERATIONS);
  retval += test <unsigned short, int> (ACE_TEXT("unsigned short"), ITERATIONS);
  retval += test <bool> (ACE_TEXT("bool"), ITERATIONS);
#if !defined (ACE_LACKS_LONGLONG_T)
  retval += test <long long, int> (ACE_TEXT("long long"), ITERATIONS);
#endif

#if defined (ACE_HAS_THREADS)
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing exchange with long\n")));
  Exchange_Tester<long> e1 (5);
  e1.wait ();
  retval += e1.result ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing exchange with unsigned long\n")));
  Exchange_Tester<unsigned long> e2 (5);
  e2.wait ();
  retval += e2.result ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing exchange with int\n")));
  Exchange_Tester<int> e3 (5);
  e3.wait ();
  retval += e3.result ();

#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return retval;
}

