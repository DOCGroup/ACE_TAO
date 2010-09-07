// $Id$

/**
 * @file
 *
 * This program checks if the compiler / platform supports the
 * std::vector container.  The motivation for this test was a discussion
 * on the development mailing list, and the documentation was captured
 * in:
 *
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=3715
 *
 */

#include "test_config.h"

// The first part of the test is to compile this line.  If the program
// does not compile the platform is just too broken.
#include <vector>

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_03_Test"));

  // As usual, the exit status from the test is 0 on success, 1 on
  // failure
  int status = 0;

  // Create a simple list ...
  typedef std::vector<int> collection;
  collection c;

  // ... insert some elements ...
  c.push_back(5);
  c.push_back(4);
  c.push_back(3);
  c.push_back(2);
  c.push_back(1);

  // ... add all the numbers to validate that they are there ...
  int sum = 0;
  for(collection::iterator i = c.begin(), end = c.end();
      i != end;
      ++i)
    {
      sum += *i;
    }

  // ... remember Euler ...
  int const expected = 5*(5+1)/2;
  if (sum != expected)
    {
      status = 1;
      ACE_ERROR ((LM_ERROR, ACE_TEXT("Expected %d got %d\n"),
                  expected, sum));
    }

  ACE_END_TEST;
  return status;
}


