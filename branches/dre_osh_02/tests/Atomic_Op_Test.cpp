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
#include "ace/Atomic_Op.h"

ACE_RCSID(tests, Atomic_Op_Test, "$Id$")

#if defined (ACE_HAS_THREADS)

enum { TEST_ITERATIONS = 1000000 };

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Atomic_Op_Test"));

  ACE_Atomic_Op <ACE_Thread_Mutex, long> foo (5);

  ACE_ASSERT (foo == 5);

  long result = ++foo;
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

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <long> assignment %D\n")));
  int i;
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      foo = 1;
      foo = 2;
      foo = 3;
      foo = 4;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <long> assignment %D\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <long> increment %D\n")));
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      ++foo;
      ++foo;
      ++foo;
      ++foo;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <long> increment %D\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <long> decrement %D\n")));
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      --foo;
      --foo;
      --foo;
      --foo;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <long> decrement %D\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <long> addition %D\n")));
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      foo += 5;
      foo += 5;
      foo += 5;
      foo += 5;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <long> addition %D\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <long> subtraction %D\n")));
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      foo -= 5;
      foo -= 5;
      foo -= 5;
      foo -= 5;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <long> subtraction %D\n")));

  ACE_Atomic_Op <ACE_Thread_Mutex, int> bar (5);

  ACE_ASSERT (bar == 5);

  result = ++bar;
  ACE_ASSERT (bar == 6);
  ACE_ASSERT (result == 6);

  result = --bar;
  ACE_ASSERT (bar == 5);
  ACE_ASSERT (result == 5);

  result = bar++;
  ACE_ASSERT (bar == 6);
  ACE_ASSERT (result == 5);

  result = bar--;
  ACE_ASSERT (bar == 5);
  ACE_ASSERT (result == 6);

  result = bar += 10;
  ACE_ASSERT (bar == 15);
  ACE_ASSERT (result == 15);

  result = bar -= 10;
  ACE_ASSERT (bar == 5);
  ACE_ASSERT (result == 5);

  bar = 5L;
  ACE_ASSERT (bar == 5);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <int> assignment %D\n")));
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      bar = 1;
      bar = 2;
      bar = 3;
      bar = 4;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <int> assignment %D\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <int> increment %D\n")));
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      ++bar;
      ++bar;
      ++bar;
      ++bar;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <int> increment %D\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <int> decrement %D\n")));
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      --bar;
      --bar;
      --bar;
      --bar;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <int> decrement %D\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <int> addition %D\n")));
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      bar += 5;
      bar += 5;
      bar += 5;
      bar += 5;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <int> addition %D\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting <int> subtraction %D\n")));
  for (i = 0; i < TEST_ITERATIONS; ++i)
    {
      bar -= 5;
      bar -= 5;
      bar -= 5;
      bar -= 5;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending <int> subtraction %D\n")));

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<ACE_Thread_Mutex, int>;
template class ACE_Atomic_Op_Ex<ACE_Thread_Mutex, int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, int>
#pragma instantiate ACE_Atomic_Op_Ex<ACE_Thread_Mutex, int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Atomic_Op_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_THREADS */
