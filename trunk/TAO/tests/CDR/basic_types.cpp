//
// $Id$
//

#include "tao/corba.h"

int
main (int , char *[])
{
  CDR cdr;

  CORBA::Octet o = 1;
  CORBA::Short s = 2;
  CORBA::Long  l  = 4;

  const int n = 4096;
  for (int i = 0; i < n; ++i)
    {
      if (cdr.put_octet (o) == 0)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "put_octet[%d] failed\n", i), 1);
	}
      if (cdr.put_short (s) == 0)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "put_short[%d] failed\n", i), 1);
	}
      if (cdr.put_octet (o) == 0)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "put_octet-2[%d] failed\n", i), 1);
	}
      if (cdr.put_long (l) == 0)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "put_long[%d] failed\n", i), 1);
	}
      if (cdr.put_long (l) == 0)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "put_long-2[%d] failed\n", i), 1);
	}
    }

  CORBA::Octet xo;
  CORBA::Short xs;
  CORBA::Long  xl;

  for (int j = 0; j < n; ++j)
    {
      if (cdr.get_octet (xo) == 0)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "get_octet[%d] failed\n", j), 1);
	}
      if (xo != o)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "octet[%d] differs\n", j), 1);
	}
      if (cdr.get_short (xs) == 0)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "get_short[%d] failed\n", j), 1);
	}
      if (xs != s)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "short[%d] differs\n", j), 1);
	}
      if (cdr.get_octet (xo) == 0)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "get_octet-2[%d] failed\n", j), 1);
	}
      if (xo != o)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "octet-2[%d] differs\n", j), 1);
	}
      if (cdr.get_long (xl) == 0)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "get_long[%d] failed\n", j), 1);
	}
      if (xl != l)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "long[%d] differs\n", j), 1);
	}
      if (cdr.get_long (xl) == 0)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "get_long-2[%d] failed\n", j), 1);
	}
      if (xl != l)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "long-2[%d] differs\n", j), 1);
	}
    }

  return 0;
}
