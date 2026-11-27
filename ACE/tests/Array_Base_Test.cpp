/**
 * @file Array_Base_Test.cpp
 *
 * Verify the copy behavior of ACE_Array_Base.
 */

#include "test_config.h"
#include "ace/Array_Base.h"

#include <type_traits>

static_assert (!std::is_move_constructible_v<ACE_Array_Base<int>>);
static_assert (!std::is_move_assignable_v<ACE_Array_Base<int>>);

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Array_Base_Test"));

  ACE_Array_Base<int> source (2, 42);
  ACE_Array_Base<int> constructed (source);
  ACE_Array_Base<int> assigned;
  assigned = source;

  int result = 0;
  if (source.size () != 2 || constructed.size () != 2 || assigned.size () != 2
      || source[0] != 42 || constructed[0] != 42 || assigned[0] != 42
      || source[1] != 42 || constructed[1] != 42 || assigned[1] != 42)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Copying the array failed\n")));
      result = 1;
    }

  ACE_END_TEST;
  return result;
}
