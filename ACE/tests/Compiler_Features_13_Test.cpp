// $Id$

/**
 * @file
 *
 * This program checks if the compiler / platform supports the
 * standard cast operators template parameters.  The motivation for
 * this test was a discussion on the development mailing list, and the
 * documentation was captured in:
 *
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=3715
 *
 */

#include "test_config.h"

#include <stdexcept>

namespace
{
  /**
   * Helper class for test
   */
  struct Base
  {
    virtual ~Base()
    {}
  };

  /**
   * Helper class for test
   */
  struct Derived : public Base
  {
    int value;
  };

  /**
   * Helper class for test
   */
  struct Another : public Base
  {
    int x;
    int y;
  };
}


int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_13_Test"));

  // As usual, the exit status from the test is 0 on success, 1 on
  // failure
  int status = 0;

  {
    // Make sure const cast works.  Compilation is interesting, the
    // functionality test here is just to make sure the compiler does
    // not optimize things away ...
    int x = 5;
    int const & y = x;
    const_cast<int&>(y) = 3;

    if (x != 3)
      {
        status = 1;
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("Wrong value after const_cast,")
                   ACE_TEXT(" expected %d, got %d\n"),
                   3, x));
      }
  }

  // Make sure dynamic cast through pointers work ...
  Derived d;
  d.value = 24;
  Base * b1 = &d;
  Derived * d1 = dynamic_cast<Derived*>(b1);
  if (d1 == 0)
    {
      status = 1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("dynamic_cast returns null, expected value\n")));
    }
  d1->value = 42;
  if (d.value != 42)
    {
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("Wrong value after dynamic_cast, expected %d, got %d\n"),
                 42, d.value));
    }

  // Make sure dynamic cast detects invalid casts
  Another a;
  Base * b2 = &a;
  Derived * d2 = dynamic_cast<Derived*>(b2);
  if (d2 != 0)
    {
      status = 1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("dynamic_cast should return null\n")));
    }

  // Make sure dynamic cast raises an exception
  Base & b3 = a;
  try
    {
      (void) dynamic_cast<Derived&>(b3);

      status = 1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("dynamic_cast should have raised exception\n")));
    }
  catch(std::exception const &)
    {
    }
  catch(...)
    {
      status = 1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("dynamic_cast should have raised std::exception\n")));
    }

  {
    // Just test these compile ...
    double x = 42.0;
    int y = static_cast<int>(x);
    void * z = reinterpret_cast<void*>(y);

    if (z == 0)
      {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("My hack to make sure the code is not ")
                   ACE_TEXT("optimized away backfired!\n")));
      }
  }

  ACE_END_TEST;
  return status;
}

