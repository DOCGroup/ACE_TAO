/**
 * @file
 *
 * This program checks if the compiler / platform supports template
 * template parameters.  The motivation for this test was a discussion
 * on the development mailing list, and the documentation was captured
 * in:
 *
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=3715
 */

#include "test_config.h"

// Similar to Bug_3709_Regression_Test.cpp...

// HP aC++ 03.x fails this
#if defined(__HP_aCC) && (__HP_aCC < 40000)
#define BROKEN_TEMPLATE_TEMPLATE
#endif

#ifndef BROKEN_TEMPLATE_TEMPLATE

template<typename T>
struct Pair
{
  T x1;
  T x2;
};

template<typename T>
struct Triple
{
  T t1;
  T t2;
  T t3;
};

template<typename T, template <typename> class Tuple>
struct Array
{
  Tuple<T> array[5];
};

#endif /* BROKEN_TEMPLATE_TEMPLATE */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_12_Test"));

  // As usual, the exit status from the test is 0 on success, 1 on
  // failure
  int status = 0;

#ifndef BROKEN_TEMPLATE_TEMPLATE

  Array<int, Pair> pairs;
  pairs.array[0].x1 = 0;
  ACE_UNUSED_ARG (pairs);

  Array<int, Triple> triples;
  triples.array[1].t3 = 0;
  ACE_UNUSED_ARG (triples);

#endif /* BROKEN_TEMPLATE_TEMPLATE */

  ACE_END_TEST;
  return status;
}
