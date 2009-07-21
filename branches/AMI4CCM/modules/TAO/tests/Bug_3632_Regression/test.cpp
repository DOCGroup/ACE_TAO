// $Id$

#include "testC.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  Test::bounded_string_seq seq;
  seq.length (11);
  ACE_TEST_ASSERT (seq.maximum () == 10);

  return 0;
}
