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

#include "tao/ORB.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "tao/AnyTypeCode/Any.h"

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

static int n = 4096;
static int nloops = 100;

struct CDR_Test_Types
{
  CDR_Test_Types (void);

  CORBA::Octet o;
  CORBA::Short s;
  CORBA::Long l;
  const char* str;
  CORBA::Double d;
  CORBA::Any any;

  enum {
    ARRAY_SIZE = 10
  };

  CORBA::Short a[ARRAY_SIZE];
};

CDR_Test_Types::CDR_Test_Types (void)
  : o (1), s (2), l (4),
    str ("abc"), d (8)
{
  any <<= CORBA::Short (s);

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
      if (!(cdr << test_types.any))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "encode Any [%d] failed\n",
                           i),
                          1);
    }

  return 0;
}

static int
test_get (TAO_InputCDR &cdr, const CDR_Test_Types &test_types)
{
  CORBA::Octet xo;
  CORBA::Short xs;
  CORBA::Long xl;
  CORBA::Any any;

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
      CORBA::String xstr;
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
      if (cdr >> any)
        {
          CORBA::Short s;
          if (any >>= s)
            {
              if (test_types.s != s)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "Any short mismatch [%d != %d in loop %d]\n",
                                   test_types.s,
                                   s,
                                   i),
                                  1);
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Any did not receive a short [%d]\n",
                               i),
                              1);
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Any decode failed [%d]\n",
                           i),
                          1);
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("dn:l:"));
      int opt;

      while ((opt = get_opt ()) != EOF)
              {
                switch (opt)
                  {
                    case 'd':
                      TAO_debug_level++;
                      break;
                    case 'n':
                      n = ACE_OS::atoi (get_opt.opt_arg ());
                      break;
                    case 'l':
                      nloops = ACE_OS::atoi (get_opt.opt_arg ());
                      break;
                    case '?':
                    default:
                      ACE_DEBUG ((LM_DEBUG,
                                  "Usage: %s "
                                  "-d debug"
                                  "-n <num> "
                                  "-l <loops> "
                                  "\n",
                                  argv[0]));
                      return -1;
                    }
              }

      for (int i = 0; i < nloops; ++i)
              {
                TAO_OutputCDR output;
                CDR_Test_Types test_types;

                if (test_put (output, test_types) != 0)
                  {
                    return 1;
                  }
                TAO_InputCDR input (output);
                if (TAO_debug_level > 0)
                  {
                    ACE_DEBUG ((LM_DEBUG, "Output CDR:\n"));
                    ACE_HEX_DUMP ((LM_DEBUG, input.rd_ptr(), 64));
                    ACE_DEBUG ((LM_DEBUG, "Input CDR:\n"));
                    ACE_HEX_DUMP ((LM_DEBUG, input.rd_ptr(), 64));
                  }

                if (test_get (input, test_types) != 0)
                  {
                    return 1;
                  }
              }

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Basic_Types");
      return 1;
    }

  return 0;
}
