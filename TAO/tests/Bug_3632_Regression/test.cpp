#include "testC.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  bool bad_param_caught {};
  try
    {
      Test::bounded_string_seq seq;
      seq.length (11);
      ACE_TEST_ASSERT (seq.maximum () == 10);
    }
  catch (::CORBA::BAD_PARAM const &)
    {
      bad_param_caught = true;
    }
  catch (::CORBA::Exception const &)
    {
      return 1;
    }

  ACE_TEST_ASSERT (bad_param_caught);
  return 0;
}
