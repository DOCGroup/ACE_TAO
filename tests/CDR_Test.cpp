// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    CDR_Test.cpp
//
// = DESCRIPTION
//    Checks the functionality of the ACE CDR streams.
//
// = AUTHORS
//    Istvan Buki <istvan.buki@euronet.be> and
//    Jeff Parsons <parsons@cs.wustl.edu> and
//    Carlos O'Ryan <coryan@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/CDR_Stream.h"

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

static int n = 4096;
static int nloops = 100;

struct CDR_Test_Types
{
  CDR_Test_Types (void);

  CDR::Octet o;
  CDR::Short s;
  CDR::Long l;
  const CDR::Char *str;
  CDR::Double d;

  enum
  {
    ARRAY_SIZE = 10
  };

  CDR::Short a[ARRAY_SIZE];
};

CDR_Test_Types::CDR_Test_Types (void)
  : o (1),
    s (2),
    l (4),
    str ("abc"),
    d (8)
{
  for (int i = 0;
       i < CDR_Test_Types::ARRAY_SIZE;
       ++i)
    a[i] = i;
}

static int
test_put (ACE_OutputCDR &cdr,
          CDR_Test_Types &test_types)
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

      if (cdr.write_string (test_types.str) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "write_string[%d] failed\n",
                           i),
                          1);

    }

  return 0;
}

static int
test_get (ACE_InputCDR &cdr, const CDR_Test_Types &test_types)
{
  CDR::Octet xo;
  CDR::Short xs;
  CDR::Long xl;

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

      CDR::Char *xstr;
      if (cdr.read_string (xstr) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "read_string2[%d] failed\n",
                           i),
                          1);
      ACE_Auto_Basic_Array_Ptr<CDR::Char> auto_xstr (xstr);
      if (ACE_OS::strcmp (auto_xstr.get (), test_types.str) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "string[%d] differs\n",
                           i),
                          1);
    }
  return 0;
}

static int
short_stream (void)
{
  // counter
  u_int i;

  // Build an output stream
  ACE_OutputCDR os;

  // Basic types for output
  CDR::Char ch = 'A';
  ACE_CString str ("Test String");
  CDR::Short s = -123;
  CDR::UShort us =  123;
  CDR::Long l = -65800L;
  CDR::ULong ul =  65800UL;
  CDR::Float f =  1.23f;
  CDR::Double d =  123.456789;

  // Arrays for output
  CDR::Short s_array[3] = { -1, 0, 1 };
  CDR::Long l_array[3] = { -345678, 0, 345678 };
  CDR::Float f_array[3] = { -1.23f, 0.0f, 1.23f };
  CDR::Double d_array[3] = { -123.456789, 0.0, 123.456789 };

  ACE_OutputCDR::from_char fc (ch);
  os << fc;
  os << str;
  os << s;
  os << us;
  os << l;
  os << ul;
  os << f;
  os << d;
  os.write_short_array (s_array, 3);
  os.write_long_array (l_array, 3);
  os.write_float_array (f_array, 3);
  os.write_double_array (d_array, 3);

  const ACE_Message_Block *out_mb = os.begin ();
  u_int len = out_mb->length ();

  // Create an input stream (copy constructor)
  ACE_InputCDR is (os);
  const ACE_Message_Block *in_mb = is.start ();

  if (in_mb->length () != len)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "buffer length not preserved"),
                      1);

  u_long in_chunk, out_chunk;

  for (i = 0; i < len; i++)
    {
      in_chunk = u_long (* (in_mb->rd_ptr () + i));
      out_chunk = u_long (* (out_mb->rd_ptr () + i));
      if (in_chunk != out_chunk )
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "buffer contents not preserved"),
                          1);
    }

  // Basic types for input
  CDR::Char ch1 = '\0';
  ACE_CString str1;
  CDR::Short s1 = 0;
  CDR::UShort us1 = 0;
  CDR::Long l1 = 0L;
  CDR::ULong ul1 = 0UL;
  CDR::Float f1 = 0.0f;
  CDR::Double d1 = 0.0;

  // Arrays for input
  CDR::Short s_array1[3];
  CDR::Long l_array1[3];
  CDR::Float f_array1[3];
  CDR::Double d_array1[3];

  ACE_DEBUG ((LM_DEBUG,
              "Checking operators and arrays\n\n"));

  ACE_InputCDR::to_char tc (ch1);
  is >> tc;
  is >> str1;
  is >> s1;
  is >> us1;
  is >> l1;
  is >> ul1;
  is >> f1;
  is >> d1;
  is.read_short_array (s_array1, 3);
  is.read_long_array (l_array1, 3);
  is.read_float_array (f_array1, 3);
  is.read_double_array (d_array1, 3);

  if (ch1 != ch)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "char transfer error"),
                      1);

  if (str1 != str)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "string transfer error"),
                      1);

  if (s1 != s)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "short transfer error"),
                      1);

  if (us1 != us)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ushort transfer error"),
                      1);

  if (l1 != l)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "long transfer error"),
                      1);

  if (ul1 != ul)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ulong transfer error"),
                      1);

  if (f1 != f)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "float transfer error"),
                      1);

  if (d1 != d)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "double transfer error"),
                      1);

  for (i = 0 ; i < 3; i++)
    if (s_array1[i] != s_array[i])
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "short array transfer error"),
                        1);

  for (i = 0 ; i < 3; i++)
    if (l_array1[i] != l_array[i])
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "long array transfer error"),
                        1);

  for (i = 0 ; i < 3; i++)
    if (f_array1[i] != f_array[i])
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "float array transfer error"),
                        1);

  for (i = 0 ; i < 3; i++)
    if (d_array1[i] != d_array[i])
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "double array transfer error"),
                        1);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
// Not necessary here, because it's instantiated in ace/Memory_Pool.cpp.
// template class ACE_Auto_Basic_Array_Ptr<CDR::Char>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
// Not necessary here, because it's instantiated in ace/Memory_Pool.cpp.
// #pragma instantiate ACE_Auto_Basic_Array_Ptr<CDR::Char>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("CDR_Test"));

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE::major_version (),
              ACE::minor_version(),
              ACE::beta_version()));

  ACE_Get_Opt get_opt (argc, argv, "dn:l:");
  int opt, debug = 0;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
          case 'd':
            debug++;
            break;
          case 'n':
            n = ACE_OS::atoi (get_opt.optarg);
            break;
          case 'l':
            nloops = ACE_OS::atoi (get_opt.optarg);
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

  ACE_DEBUG ((LM_DEBUG,
              "Testing ACE CDR functions - short stream\n\n"));

  if (short_stream () != 0 )
    return 1;

  ACE_DEBUG ((LM_DEBUG,
              "Short stream - no errors\n\n"
              "Testing basic types - long stream\n\n"));

  for (int i = 0; i < nloops; ++i)
    {
      ACE_OutputCDR output;
      CDR_Test_Types test_types;

      if (test_put (output, test_types) != 0)
        return 1;

      ACE_InputCDR input (output);
      if (debug > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Output CDR: \n"));
          ACE_HEX_DUMP ((LM_DEBUG,
                         input.rd_ptr(),
                         64));
          ACE_DEBUG ((LM_DEBUG,
                      "Input CDR: \n"));
          ACE_HEX_DUMP ((LM_DEBUG,
                         input.rd_ptr(),
                         64));
        }

      if (test_get (input, test_types) != 0)
        return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Long stream - no errors\n\n"));

  ACE_END_TEST;

  return 0;
}
