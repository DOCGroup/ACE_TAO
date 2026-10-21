#include "tao/SystemException.h"

int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  int result {};
  try
    {
      throw CORBA::BAD_PARAM ();
    }
  catch (CORBA::BAD_PARAM const &)
    {
      // Correct exception
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Incorrect exception caught:");
      ++result;
    }

  return result;
}
