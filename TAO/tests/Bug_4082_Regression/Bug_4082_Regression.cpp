#include "Bug_4082_RegressionC.h"
#include <tao/AnyTypeCode/TypeCode.h>

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  MyLongSeq mls (2);
  mls.length (2);
  mls[0] = 0;
  mls[1] = 1;
  CORBA::Any a;
  a <<= mls;

  CORBA::TypeCode_var tc = a.type ();
  if (!tc->equivalent (CORBA::_tc_LongSeq) || tc->equal (CORBA::_tc_LongSeq))
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("ERROR: Test precondition not met\n")));
      return 1;
    }

  const CORBA::LongSeq* cls = 0;
  if (!(a >>= cls))
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("ERROR: Failed to extract\n")));
      return 1;
    }

  if (!cls || cls->length () != 2 || (*cls)[0] != 0 || (*cls)[1] != 1)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("ERROR: Extract invalid data\n")));
      return 1;
    }

  return 0;
}
