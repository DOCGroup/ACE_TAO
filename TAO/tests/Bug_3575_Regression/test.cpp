#include "tao/StringSeqC.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  char const *str = "Some string";
  constexpr CORBA::ULong sequence_length = 1;

  CORBA::StringSeq seq;
  seq.length (sequence_length);
  seq[0] = str;
  char const * const before_ptr = seq[0].in ();
  CORBA::String_var tmp = seq[0]._retn ();
  // The pointer returned by in() and _retn() must be the same.
  ACE_TEST_ASSERT (before_ptr == tmp.in ());

  return 0;
}
