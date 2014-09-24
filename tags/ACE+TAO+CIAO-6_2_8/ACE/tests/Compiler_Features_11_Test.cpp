// $Id$

/**
 * @file
 *
 * This program checks if the compiler / platform supports the
 * string steams.  The motivation for this test was a discussion
 * on the development mailing list, and the documentation was captured
 * in:
 *
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=3715
 *
 */

#include "test_config.h"

// The first part of the test is to compile this line.  If the program
// does not compile the platform is just too broken.
#include <sstream>

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_11_Test"));

  // As usual, the exit status from the test is 0 on success, 1 on
  // failure
  int status = 0;

  // Test creation of a output stream ...
  std::ostringstream os;

  // ... add some strings and numbers to it ...
  os << "1" << 2 << 3 << "45";

  // ... verify the right thing comes out ...
  std::string const expected("12345");
  if (os.str() != expected)
    {
      status = 1;
      ACE_ERROR ((LM_ERROR, ACE_TEXT("Expected %s got %s\n"),
                  expected.c_str(), os.str().c_str()));
    }

  // ... create an input stream from the result ...
  std::istringstream is(os.str());

  // ... extract as a number ...
  int v;
  is >> v;

  // ... verify the right thing comes out ...
  if (v != 12345)
    {
      status = 1;
      ACE_ERROR ((LM_ERROR, ACE_TEXT("Expected %d got %d\n"),
                  12345, v));
    }

  ACE_END_TEST;
  return status;
}


