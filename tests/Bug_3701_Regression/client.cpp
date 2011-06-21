// $Id$

#include "tao/SystemException.h"

int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  int result = 0;
  try
    {
      throw CORBA::BAD_PARAM ();
    }
  catch (const CORBA::BAD_PARAM&)
    {
      // Correct exception
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Incorrect exception caught:");
      ++result;
    }

  return result;
}
