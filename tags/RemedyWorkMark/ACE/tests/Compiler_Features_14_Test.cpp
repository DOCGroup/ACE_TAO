// $Id$

/**
 * @file
 *
 * This program checks if the compiler / platform supports partial
 * template specialization.  The motivation for this test was a
 * discussion on the development mailing list, and the documentation
 * was captured in:
 *
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=3715
 *
 */

#include "test_config.h"

// We are going to test if partial template specializations work by
// demonstrating a standard technique in generic programming, i.e.,
// using the specialization to detect if a type is a pointer.
//
// My implementation here is not very elegant, I would even say
// ackward, and should not be taken as representative of good generic
// programming techniques.  I just wanted to through something
// together.
//

// First some helper types in the anonymous namespace
struct true_type {};
struct false_type {};

// Now a generic function to convert the types to booleans, moving
// from generic type-based programming to classical value-based
// programming.
template<class T>
bool to_boolean(T const&);

template<>
bool to_boolean(true_type const &)
{
  return true;
}

template<>
bool to_boolean(false_type const &)
{
  return false;
}

// Here is the template, by default return false for all types.
// Notice that this is a type *function*, it takes a type and returns
// another type.
template<typename T>
struct is_pointer_function
{
  false_type result;
};

// Here is the specialization, for a class of types it results
// something different.  Effectively this is an implicit if() test on
// the types.
template<typename T>
struct is_pointer_function<T*>
{
  true_type result;
};

// And here is a helper to convert back to values...
struct test
{
  template<typename T>
  static bool is_pointer()
  {
    is_pointer_function<T> v;
    return to_boolean(v.result);
  }
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_14_Test"));

  // As usual, the exit status from the test is 0 on success, 1 on
  // failure
  int status = 0;

  if (test::is_pointer<int>())
    {
      status = 1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("int should not be a pointer\n")));
    }

  if (! test::is_pointer<int*>())
    {
      status = 1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("int* should be a pointer\n")));
    }

  if (test::is_pointer<int&>())
    {
      status = 1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("int& should not be a pointer\n")));
    }

  ACE_END_TEST;
  return status;
}


