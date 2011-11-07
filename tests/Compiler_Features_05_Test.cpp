// $Id$

/**
 * @file
 *
 * This program checks if the compiler / platform supports the
 * std::queue container.  The motivation for this test was a discussion
 * on the development mailing queue, and the documentation was captured
 * in:
 *
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=3715
 *
 */

#include "test_config.h"

// The first part of the test is to compile this line.  If the program
// does not compile the platform is just too broken.
#include <queue>



int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_05_Test"));

  // As usual, the exit status from the test is 0 on success, 1 on
  // failure
  int status = 0;

  // Create a simple queue ...
  typedef std::queue<int> collection;
  // ... if the previous line compiles, the default template
  // parameters work.  The declaration of queue<> is something like:
  //   template<typename T, typename Sequence = deque<T> >
  // notice that this is not a template template parameter...
  collection c;

  // ... insert some elements ...
  c.push(5);
  c.push(4);
  c.push(3);
  c.push(2);
  c.push(1);

  // ... add all the numbers to validate that they are there ...
  int sum = 0;
  while (! c.empty())
    {
      sum += c.front();
      c.pop();
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


