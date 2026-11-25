/**
 * @file Array_Base_Test.cpp
 *
 * Verify that rvalues retain the copy behavior of ACE_Array_Base.
 */

#include "test_config.h"
#include "ace/Array_Base.h"

#include <utility>

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Array_Base_Test"));

  ACE_Array_Base<int> source (2, 42);
  ACE_Array_Base<int> constructed (std::move (source));
  ACE_Array_Base<int> assigned;
  assigned = std::move (source);

  int result = 0;
  if (source.size () != 2 || constructed.size () != 2 || assigned.size () != 2
      || source[0] != 42 || constructed[0] != 42 || assigned[0] != 42
      || source[1] != 42 || constructed[1] != 42 || assigned[1] != 42)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Copying from rvalues failed\n")));
      result = 1;
    }

  ACE_END_TEST;
  return result;
}
