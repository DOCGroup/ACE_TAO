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

#include "test_config.h"

ACE_RCSID(tests, Atomic_Op_Test, "$Id$")

#include "ace/Atomic_Op.h"
#include "ace/Synch_Traits.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h"

template <typename TYPE, typename dummy>
int test (const ACE_TCHAR* type, int iterations)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Measuring %d iterations for %s\n"), iterations, type));

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
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Substraction failed\n")));
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

  int retval = 0;

  retval += test <int, int> (ACE_TEXT("int"), 1000000);
  retval += test <long, int> (ACE_TEXT("long"), 1000000);
  retval += test <unsigned int, int> (ACE_TEXT("unsigned int"), 1000000);
  retval += test <unsigned long, int> (ACE_TEXT("unsigned long"), 1000000);
  retval += test <short, int> (ACE_TEXT("short"), 10000);
  retval += test <unsigned short, int> (ACE_TEXT("unsigned short"), 10000);
  retval += test <bool> (ACE_TEXT("bool"), 1000000);
#if !defined (ACE_LACKS_LONGLONG_T)
  retval += test <long long, int> (ACE_TEXT("long long"), 1000000);
#endif

  ACE_END_TEST;
  return retval;
}

