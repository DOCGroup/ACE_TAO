// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Atomic_Op_Test.cpp
//
// = DESCRIPTION
//      This is a simple test of the Atomic Operations Class in ACE.
//      On platforms like Win32, ACE uses template specialization to
//      use native implementations provided by the OS to accelarate
//      these operations.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "tests/test_config.h"

ACE_RCSID(tests, Atomic_Op_Test, "$Id$")

#include "ace/Atomic_Op.h"
#include "ace/Synch_Traits.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h"

enum { TEST_ITERATIONS = 1000000 };

template <typename TYPE>
int test (const ACE_TCHAR* type)
{
  int retval = 0;
  ACE_Atomic_Op <ACE_SYNCH_MUTEX, TYPE> foo (5);

  ACE_ASSERT (foo == 5);

  TYPE result = ++foo;
  ACE_ASSERT (foo == 6);
  ACE_ASSERT (result == 6);

  result = --foo;
  ACE_ASSERT (foo == 5);
  ACE_ASSERT (result == 5);

  result = foo++;
  ACE_ASSERT (foo == 6);
  ACE_ASSERT (result == 5);

  result = foo--;
  ACE_ASSERT (foo == 5);
  ACE_ASSERT (result == 6);

  result = foo += 10;
  ACE_ASSERT (foo == 15);
  ACE_ASSERT (result == 15);

  result = foo -= 10;
  ACE_ASSERT (foo == 5);
  ACE_ASSERT (result == 5);

  foo = 7;
  ACE_ASSERT (foo == 7);

  ACE_Atomic_Op <ACE_SYNCH_MUTEX, TYPE> foo2 (5);
  foo2 = foo;
  ACE_ASSERT (foo == 7);
  ACE_ASSERT (foo2 == 7);

  ACE_UINT64 usec;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> assignment %D\n"), type));
  ACE_Time_Value diff = ACE_OS::gettimeofday ();
  int i;
  for (i = 0; i < TEST_ITERATIONS; ++i)
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

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> increment %D\n"), type));
  diff = ACE_OS::gettimeofday ();
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      ++foo;
      ++foo;
      ++foo;
      ++foo;
    }
  diff = ACE_OS::gettimeofday () - diff;
  diff.to_usec (usec);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <%s> increment %D, took %Q\n"), type, usec));

  if (foo != TEST_ITERATIONS * 4)
    {
      ++retval;
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Increment failed\n")));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> decrement %D\n"), type));
  diff = ACE_OS::gettimeofday ();
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      --foo;
      --foo;
      --foo;
      --foo;
    }
  diff = ACE_OS::gettimeofday () - diff;
  diff.to_usec (usec);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <%s> decrement %D, took %Q\n"), type, usec));

  if (foo != 0)
    {
      ++retval;
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Decrement failed\n")));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> addition %D\n"), type));
  diff = ACE_OS::gettimeofday ();
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      foo += 5;
      foo += 5;
      foo += 5;
      foo += 5;
    }
  diff = ACE_OS::gettimeofday () - diff;
  diff.to_usec (usec);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <%s> addition %D, took %Q\n"), type, usec));

  if (foo != TEST_ITERATIONS * 4 * 5)
    {
      ++retval;
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Addition failed\n")));
     }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <%s> subtraction %D\n"), type));
  diff = ACE_OS::gettimeofday ();
  for (i = 0; i < TEST_ITERATIONS; ++i)
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
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Substraction failed\n")));
    }

  return retval;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Atomic_Op_Test"));

  int retval = 0;

  retval += test <int> (ACE_TEXT("int"));
  retval += test <long> (ACE_TEXT("long"));
  retval += test <unsigned int> (ACE_TEXT("unsigned int"));
  retval += test <unsigned long> (ACE_TEXT("unsigned long"));
#if !defined (ACE_LACKS_LONGLONG_T)
  retval += test <long long> (ACE_TEXT("long long"));
#endif

  ACE_END_TEST;
  return retval;
}

