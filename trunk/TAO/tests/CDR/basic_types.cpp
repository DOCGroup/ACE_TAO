// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/CDR
//
// = FILENAME
//    basic_types.cpp
//
// = DESCRIPTION
//
// = AUTHORS
//    Carlos O'Ryan
//
// ============================================================================

#include "tao/corba.h"

static const int n = 4096;

struct CDR_Test_Types
{
  CDR_Test_Types (void):
    o (1), s (2), l (4) {}

  CORBA::Octet o;
  CORBA::Short s;
  CORBA::Long l;
};

static int
test_put (CDR &cdr, CDR_Test_Types &test_types)
{
  for (int i = 0; i < n; ++i)
    {
      if (cdr.put_octet (test_types.o) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "put_octet[%d] failed\n",
                           i),
                          1);
      if (cdr.put_short (test_types.s) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "put_short[%d] failed\n",
                           i),
                          1);
      if (cdr.put_octet (test_types.o) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "put_octet-2[%d] failed\n",
                           i),
                          1);
      if (cdr.put_long (test_types.l) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "put_long[%d] failed\n",
                           i),
                          1);
      if (cdr.put_long (test_types.l) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "put_long-2[%d] failed\n",
                           i),
                          1);
    }

  return 0;
}

static int
test_get (CDR &cdr, const CDR_Test_Types &test_types)
{
  CORBA::Octet xo;
  CORBA::Short xs;
  CORBA::Long xl;

  for (int i = 0; i < n; ++i)
    {
      if (cdr.get_octet (xo) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "get_octet[%d] failed\n",
                           i),
                          1);
      if (xo != test_types.o)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "octet[%d] differs\n",
                           i),
                          1);
      if (cdr.get_short (xs) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "get_short[%d] failed\n",
                           i), 1);
      if (xs != test_types.s)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "short[%d] differs\n",
                           i),
                          1);
      if (cdr.get_octet (xo) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "get_octet-2[%d] failed\n",
                           i),
                          1);
      if (xo != test_types.o)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "octet-2[%d] differs\n",
                           i),
                          1);
      if (cdr.get_long (xl) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "get_long[%d] failed\n",
                           i),
                          1);
      if (xl != test_types.l)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "long[%d] differs\n",
                           i),
                          1);
      if (cdr.get_long (xl) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "get_long-2[%d] failed\n",
                           i),
                          1);
      if (xl != test_types.l)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "long-2[%d] differs\n",
                           i),
                          1);
    }

  return 0;
}

int
main (int, char *[])
{
  CDR cdr;
  CDR_Test_Types test_types;
 
  if (test_put (cdr, test_types) == 0 
      && test_get (cdr, test_types) == 0)
    return 0;
  else
    return 1;
}
