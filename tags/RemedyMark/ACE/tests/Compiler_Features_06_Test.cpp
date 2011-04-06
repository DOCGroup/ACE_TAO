// $Id$

/**
 * @file
 *
 * This program checks if the compiler / platform supports alternative
 * sorting functions in the std::set container.  The motivation for
 * this test was a discussion on the development mailing list, and the
 * documentation was captured
 * in:
 *
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=3715
 *
 */

#include "test_config.h"

// The first part of the test is to compile this line.  If the program
// does not compile the platform is just too broken.
#include <set>

// We are going to use std::greater<>, the spec requires us to include this
// header.  Only a few platforms enforce this though...
#include <functional>



int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_06_Test"));

  // As usual, the exit status from the test is 0 on success, 1 on
  // failure
  int status = 0;

  // Create a simple list ...
  typedef std::set<int,std::greater<int> > collection;
  collection c;

  // ... insert some elements ...
  c.insert(5);
  c.insert(4);
  c.insert(3);
  c.insert(2);
  c.insert(1);

  // ... inserting twice returns a pair ...
  std::pair<collection::iterator,bool> r =
    c.insert(collection::value_type(5));

  // ... the iterator points to the element ...
  if (*r.first != 5)
    {
      status = 1;
      ACE_ERROR ((LM_ERROR, ACE_TEXT("Expected to find 5 already in set")));
    }

  // ... and the booleans says that it is already in the set ...
  if (r.second == true)
    {
      status = 1;
      ACE_ERROR ((LM_ERROR, ACE_TEXT("Expected duplicate insert to fail")));
    }

  // ... make sure the first element is the biggest one ...
  if (5 != *c.begin())
    {
      status = 1;
      ACE_ERROR ((LM_ERROR, ACE_TEXT("Expected largest element (5) at the front")));
    }

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


