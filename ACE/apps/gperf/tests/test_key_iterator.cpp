/**
 * @file test_key_iterator.cpp
 *
 * Verify that key-position iterators keep independent range state.
 */

#include "../src/Iterator.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  char const first[] = "1-3,7";
  char const second[] = "8-10";
  Iterator left (first, 1, 126, 0, -1, -2);
  Iterator right (second, 1, 126, 0, -1, -2);

  if (left () != 1 || right () != 8
      || left () != 2 || right () != 9
      || left () != 3 || right () != 10
      || left () != 7 || right () != -2 || left () != -2)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Key iterator ranges interfered\n")), 1);
    }

  char const oversized_value[] = "999999999999999999999999999999999";
  char const oversized_range[] = "1-999999999999999999999999999999999";
  char const padded_maximum[] = "000000000000000000000000126";
  Iterator invalid_value (oversized_value, 1, 126, 0, -1, -2);
  Iterator invalid_range (oversized_range, 1, 126, 0, -1, -2);
  Iterator maximum (padded_maximum, 1, 126, 0, -1, -2);
  if (invalid_value () != -1 || invalid_range () != -1 || maximum () != 126)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Key iterator accepted an invalid number\n")), 1);
    }
  return 0;
}
