// $Id$

/**
 * @file
 *
 * This program checks if the compiler / platform supports the
 * std::set container.  The motivation for this test was a discussion
 * on the development mailing list, and the documentation was captured
 * in:
 *
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=3715
 *
 */

#include "test_config.h"

// The first part of the test is to compile this line.  If the program
// does not compile the platform is just too broken.
#include <set>
#include <string>

ACE_RCSID(tests, Compiler_Features_08_Test, "$Id$")

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_08_Test"));

  // As usual, the exit status from the test is 0 on success, 1 on
  // failure
  int status = 0;

  // Create a simple list ...
  typedef std::set<std::wstring> collection;
  collection c;

  // ... insert some elements ...
  c.insert(L"5");
  c.insert(L"4");
  c.insert(L"3");
  c.insert(L"2");
  c.insert(std::wstring(L"1"));

  // ... inserting twice returns a pair ...
  std::pair<collection::iterator,bool> r =
    c.insert(collection::value_type(L"5"));

  // ... the iterator points to the element ...
  if (*r.first != std::wstring(L"5"))
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

  // ... find an element and erase it ...
  collection::iterator i;

  i = c.find(std::wstring(L"4"));
  if (i == c.end())
    {
      status = 1;
      ACE_ERROR((LM_ERROR, ACE_TEXT("Element not found")));
    }
  else
    {
      // ... this demonstrates a standard STL technique, you can
      // optimize lookups by using the iterators returned in search
      // functions ...
      c.erase(i);
    }

  ACE_END_TEST;
  return status;
}


