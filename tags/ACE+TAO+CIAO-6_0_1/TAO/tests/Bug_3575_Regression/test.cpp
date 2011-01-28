// $Id$

#include "tao/StringSeqC.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  char const *str = "Some string";

  CORBA::StringSeq seq;
  seq.length (1);
  seq[0] = str;
  char const *beforeptr = seq[0].in ();
  CORBA::String_var tmp = seq[0]._retn ();
  // The pointer returned by in() and _retn() must be the same.
  ACE_TEST_ASSERT (beforeptr == tmp.in ());

  return 0;
}
