// $Id$

#include "testC.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  try
    {
      Test::bounded_string_seq seq;
      seq.length (11);
      ACE_TEST_ASSERT (seq.maximum () == 10);
    }
  catch (::CORBA::BAD_PARAM const &)
    {
      // ignore this exception since it's expected.
    }
  catch (::CORBA::Exception const &)
    {
      return 1;
    }

  return 0;
}
