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
//   Test the basic funcionality of a CDR stream by encoding some
//   basic types and then decoding them again.
//
// = AUTHORS
//    Carlos O'Ryan
//
// ============================================================================

#include "tao/corba.h"
#include "tao/debug.h"

static const int n = 4096;
static int nloops = 100;

struct CDR_Test_Types
{
  CDR_Test_Types (void);

  CORBA::Octet o;
  CORBA::Short s;
  CORBA::Long l;
  CORBA::String str;
  CORBA::Double d;
  
  enum {
    ARRAY_SIZE = 10
  };

  CORBA::Short a[ARRAY_SIZE];
};

CDR_Test_Types::CDR_Test_Types (void)
:
  o (1), s (2), l (4), str ("abc"), d (8)
{
  for (int i = 0; i < CDR_Test_Types::ARRAY_SIZE; ++i)
    {
      a[i] = i;
    }
}

static int
test_put (TAO_OutputCDR &cdr, CDR_Test_Types &test_types)
{
  for (int i = 0; i < n; ++i)
    {
      if (cdr.write_octet (test_types.o) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "write_octet[%d] failed\n",
                           i),
                          1);
      if (cdr.write_short (test_types.s) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "write_short[%d] failed\n",
                           i),
                          1);
      if (cdr.write_octet (test_types.o) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "write_octet-2[%d] failed\n",
                           i),
                          1);
      if (cdr.write_long (test_types.l) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "write_long[%d] failed\n",
                           i),
                          1);
      if (cdr.write_long (test_types.l) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "write_long-2[%d] failed\n",
                           i),
                          1);
#if 0
      if (cdr.write_string (test_types.str) == 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "write_string[%d] failed\n",
			   i),
			  1);
#endif
    }

  return 0;
}

static int
test_get (TAO_InputCDR &cdr, const CDR_Test_Types &test_types)
{
  CORBA::Octet xo;
  CORBA::Short xs;
  CORBA::Long xl;
  CORBA::String xstr;

  for (int i = 0; i < n; ++i)
    {
      if (cdr.read_octet (xo) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "read_octet[%d] failed\n",
                           i),
                          1);
      if (xo != test_types.o)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "octet[%d] differs\n",
                           i),
                          1);
      if (cdr.read_short (xs) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "read_short[%d] failed\n",
                           i), 1);
      if (xs != test_types.s)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "short[%d] differs\n",
                           i),
                          1);
      if (cdr.read_octet (xo) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "read_octet-2[%d] failed\n",
                           i),
                          1);
      if (xo != test_types.o)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "octet-2[%d] differs\n",
                           i),
                          1);
      if (cdr.read_long (xl) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "read_long[%d] failed\n",
                           i),
                          1);
      if (xl != test_types.l)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "long[%d] differs\n",
                           i),
                          1);
      if (cdr.read_long (xl) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "read_long-2[%d] failed\n",
                           i),
                          1);
      if (xl != test_types.l)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "long-2[%d] differs\n",
                           i),
                          1);
#if 0
      if (cdr.read_string (xstr) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "read_string2[%d] failed\n",
                           i),
                          1);
      if (ACE_OS::strcmp (xstr, test_types.str) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "string[%d] differs\n",
                           i),
                          1);
      CORBA::string_free (xstr);
#endif
    }

  return 0;
}

int
main (int, char *[])
{
  for (int i = 0; i < nloops; ++i)
    {
      TAO_OutputCDR output;
      CDR_Test_Types test_types;
 
      if (test_put (output, test_types) != 0)
	{
	  return 1;
	}
      TAO_InputCDR input (output);
#if 0
      ACE_DEBUG ((LM_DEBUG, "Output CDR: \n"));
      ACE_HEX_DUMP ((LM_DEBUG, input.rd_ptr(), 64));
      ACE_DEBUG ((LM_DEBUG, "Input CDR: \n"));
      ACE_HEX_DUMP ((LM_DEBUG, input.rd_ptr(), 64));
#endif
      if (test_get (input, test_types) != 0)
	{
	  return 1;
	}
    }
  return 0;
}
