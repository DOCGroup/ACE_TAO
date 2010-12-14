// ============================================================================
/**
 * @file Integer_Truncate_Test.cpp
 *
 * $Id$
 *
 * Test @c ACE_Utils::truncate_cast<> function template.
 *
 * @author Ossama Othman <ossama_othman at symantec dot com>
 */
// ============================================================================

#include "test_config.h"

#include <ace/Truncate.h>
#include <ace/Numeric_Limits.h>

#include <algorithm>
#include <functional>

using namespace ACE_Utils;

// ----------------------------------------------------

bool
sizeof_from_lt_sizeof_to (void)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Running sizeof(FROM) < sizeof(TO) test\n")));

  bool success = true;

  // signed from_type, unsigned to_type
  {
    typedef signed char from_type;
    typedef unsigned int to_type;

    ACE_TEST_ASSERT (sizeof (from_type) < sizeof (to_type));

    from_type f =
      ACE_Numeric_Limits<from_type>::max ();  // Should not be truncated.

    if (truncate_cast<to_type> (f) != static_cast<to_type> (f))
      {
        success = false;

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\tsigned from_type / unsigned to_type ")
                    ACE_TEXT ("truncation test failed")));
      }
  }

  // unsigned from_type, signed to_type
  {
    typedef unsigned char from_type;
    typedef signed int to_type;

    ACE_TEST_ASSERT (sizeof (from_type) < sizeof (to_type));

    from_type f =
      ACE_Numeric_Limits<from_type>::max ();  // Should not be truncated.

    if (truncate_cast<to_type> (f) != static_cast<to_type> (f))
      {
        success = false;

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\tunsigned from_type / signed to_type ")
                    ACE_TEXT ("truncation test failed")));
      }
  }

  // signed from_type, signed to_type
  {
    typedef signed char from_type;
    typedef signed int to_type;

    ACE_TEST_ASSERT (sizeof (from_type) < sizeof (to_type));

    from_type f1 = -1;  // Should not be truncated.
    from_type f2 =
      ACE_Numeric_Limits<from_type>::max ();  // Should not be truncated.

    if (truncate_cast<to_type> (f1) != f1
        || truncate_cast<to_type> (f2) != f2)
      {
        success = false;

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\tsigned from_type / signed to_type ")
                    ACE_TEXT ("truncation test failed")));
      }
  }

  // unsigned from_type, unsigned to_type
  {
    typedef unsigned char from_type;
    typedef unsigned int to_type;

    ACE_TEST_ASSERT (sizeof (from_type) < sizeof (to_type));

    from_type f =
      ACE_Numeric_Limits<from_type>::max ();  // Should not be truncated.

    if (truncate_cast<to_type> (f) != static_cast<to_type> (f))
      {
        success = false;

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\tunsigned from_type / unsigned to_type ")
                    ACE_TEXT ("truncation test failed")));
      }
  }

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("\t%s\n"),
              success
              ? ACE_TEXT ("PASSED")
              : ACE_TEXT ("FAILED")));

  return success;
}

bool
sizeof_from_eq_sizeof_to (void)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Running sizeof(FROM) == sizeof(TO) test\n")));

  bool success = true;

  // signed from_type, unsigned to_type
  {
    typedef signed int from_type;
    typedef unsigned int to_type;

    ACE_TEST_ASSERT (sizeof (from_type) == sizeof (to_type));

    from_type f1 = -1;  // Should not be truncated.
    from_type f2 =
      ACE_Numeric_Limits<from_type>::max ();  // Should not be truncated.

    if (static_cast<from_type> (truncate_cast<to_type> (f1)) != f1
        || static_cast<from_type> (truncate_cast<to_type> (f2)) != f2)
      {
        success = false;

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\tsigned from_type / unsigned to_type ")
                    ACE_TEXT ("truncation test failed")));
      }
  }

  // unsigned from_type, signed to_type
  {
    typedef unsigned int from_type;
    typedef signed int to_type;

    ACE_TEST_ASSERT (sizeof (from_type) == sizeof (to_type));

    from_type f =
      ACE_Numeric_Limits<from_type>::max ();  // Should be truncated.

    if (truncate_cast<to_type> (f) != ACE_Numeric_Limits<to_type>::max ())
      {
        success = false;

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\tunsigned from_type / signed to_type ")
                    ACE_TEXT ("truncation test failed")));
      }
  }

  // signed from_type, signed to_type
  {
    typedef signed int from_type;
    typedef signed int to_type;

    ACE_TEST_ASSERT (sizeof (from_type) == sizeof (to_type));

    from_type f1 = -1;  // Should not be truncated.
    from_type f2 =
      ACE_Numeric_Limits<from_type>::max ();  // Should not be truncated.

    if (truncate_cast<to_type> (f1) != f1
        || truncate_cast<to_type> (f2) != f2)
      {
        success = false;

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\tsigned from_type / signed to_type ")
                    ACE_TEXT ("truncation test failed")));
      }
  }

  // unsigned from_type, unsigned to_type
  {
    typedef unsigned int from_type;
    typedef unsigned int to_type;

    ACE_TEST_ASSERT (sizeof (from_type) == sizeof (to_type));

    from_type f =
      ACE_Numeric_Limits<from_type>::max ();  // Should not be truncated.

    if (truncate_cast<to_type> (f) != f)
      {
        success = false;

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\tunsigned from_type / unsigned to_type ")
                    ACE_TEXT ("truncation test failed")));
      }
  }

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("\t%s\n"),
              success
              ? ACE_TEXT ("PASSED")
              : ACE_TEXT ("FAILED")));

  return success;
}

bool
sizeof_from_gt_sizeof_to (void)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Running sizeof(FROM) > sizeof(TO) test\n")));

  bool success = true;

  // signed from_type, unsigned to_type
  {
    typedef signed int from_type;
    typedef unsigned char to_type;

    ACE_TEST_ASSERT (sizeof (from_type) > sizeof (to_type));

    from_type f =
      ACE_Numeric_Limits<from_type>::max ();  // Should be truncated.

    if (truncate_cast<to_type> (f) != ACE_Numeric_Limits<to_type>::max ())
      {
        success = false;

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\tsigned from_type / unsigned to_type ")
                    ACE_TEXT ("truncation test failed")));
      }
  }

  // unsigned from_type, signed to_type
  {
    typedef unsigned int from_type;
    typedef signed char to_type;

    ACE_TEST_ASSERT (sizeof (from_type) > sizeof (to_type));

    from_type f =
      ACE_Numeric_Limits<from_type>::max ();  // Should be truncated.

    if (truncate_cast<to_type> (f) != ACE_Numeric_Limits<to_type>::max ())
      {
        success = false;

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\tunsigned from_type / signed to_type ")
                    ACE_TEXT ("truncation test failed")));
      }
  }

  // signed from_type, signed to_type
  {
    typedef signed int from_type;
    typedef signed char to_type;

    ACE_TEST_ASSERT (sizeof (from_type) > sizeof (to_type));

    from_type f1 = -1;  // Should not be truncated.
    from_type f2 =
      ACE_Numeric_Limits<from_type>::max ();  // Should be truncated.

    if (truncate_cast<to_type> (f1) != f1
        || truncate_cast<to_type> (f2) != ACE_Numeric_Limits<to_type>::max ())
      {
        success = false;

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\tsigned from_type / signed to_type ")
                    ACE_TEXT ("truncation test failed")));
      }
  }

  // unsigned from_type, unsigned to_type
  {
    typedef unsigned int from_type;
    typedef unsigned char to_type;

    ACE_TEST_ASSERT (sizeof (from_type) > sizeof (to_type));

    from_type f =
      ACE_Numeric_Limits<from_type>::max ();  // Should be truncated.

    if (truncate_cast<to_type> (f) != ACE_Numeric_Limits<to_type>::max ())
      {
        success = false;

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("\tunsigned from_type / unsigned to_type ")
                    ACE_TEXT ("truncation test failed")));
      }
  }

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("\t%s\n"),
              success
              ? ACE_TEXT ("PASSED")
              : ACE_TEXT ("FAILED")));

  return success;
}

// ----------------------------------------------------

/**
 * @struct Caller
 *
 * @brief Test method invocation functor.
 *
 * Test method invocation functor.
 */
template <typename T>
struct Caller : public std::unary_function<T, void>
{
  /// Constructor
  Caller (void) : success (true) {}

  /// Function call operator overload.
  void operator() (T f)
  {
    if (!f ())
      success = false;
  }

  /// Flag that indicates success of all tests.
  bool success;
};

// ----------------------------------------------------

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Integer_Truncate_Test"));

  typedef bool (*test_func) ();

  static test_func const tests[] =
    {
      sizeof_from_lt_sizeof_to
      , sizeof_from_eq_sizeof_to
      , sizeof_from_gt_sizeof_to
    };

  static size_t const test_count = sizeof (tests) / sizeof (tests[0]);

  // Have some fun with the STL.  :-)
  Caller<test_func> c =
    std::for_each (tests,
                   tests + test_count,
                   Caller<test_func> ());

  ACE_END_TEST;

  return c.success ? 0 : -1;
}
