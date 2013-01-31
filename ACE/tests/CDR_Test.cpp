
//=============================================================================
/**
 *  @file    CDR_Test.cpp
 *
 *  $Id$
 *
 *  Checks the functionality of the ACE CDR streams.
 *
 *
 *  @author Istvan Buki <istvan.buki@euronet.be> and Jeff Parsons <parsons@cs.wustl.edu> and Carlos O'Ryan <coryan@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/CDR_Stream.h"
#include "ace/CDR_Size.h"
#include "ace/SString.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_wchar.h"



static int n = 4096;
static int nloops = 100;

struct CDR_Test_Types
{
  CDR_Test_Types (void);

  ACE_CDR::Octet o;
  ACE_CDR::Short s;
  ACE_CDR::Long l;
  const ACE_CDR::Char *str;
  const ACE_CDR::WChar *wstr;
  ACE_CDR::Double d;
  ACE_CDR::Short reps;
  ACE_CDR::UShort repus;
  ACE_CDR::Long repl;
  ACE_CDR::ULong repul;
  ACE_CDR::Boolean repb;
  const ACE_CDR::Char repc;
  ACE_CDR::LongLong repll;
  ACE_CDR::ULongLong repull;
  ACE_CDR::Octet repo;
  ACE_CDR::Float repf;
  ACE_CDR::Double repd;

  int test_put (ACE_OutputCDR& cdr);
  int test_get (ACE_InputCDR& cdr) const;
  int test_put_placeholder (ACE_OutputCDR& cdr);
  int test_get_placeholder (ACE_InputCDR& cdr) const;

  enum
  {
    ARRAY_SIZE = 10
  };

  ACE_CDR::Short a[ARRAY_SIZE];
};

CDR_Test_Types::CDR_Test_Types (void)
  : o (1),
    s (2),
    l (4),
    str ("abc"),
    wstr (0),
    d (8),
    reps (-123),
    repus (456),
    repl (-65800L),
    repul (65800L),
    repb (false),
    repc ('d'),
    repll (-5000000000LL),
    repull (5000000000LL),
    repo (05),
    repf (3.14F),
    repd (6.00)
{
  for (int i = 0;
       i < CDR_Test_Types::ARRAY_SIZE;
       ++i)
    a[i] = i;
}

static int
short_stream (void)
{
  // counter
  u_int i;

  // Build an output stream
  ACE_OutputCDR os;
  ACE_SizeCDR ss;

  // Basic types for output
  ACE_CDR::Char ch = 'A';
  ACE_CDR::Char wchtmp[] = {'\xF3'};
  ACE_CDR::WChar wch = *wchtmp;
  ACE_CDR::WChar wchar2[] = {'\x00'};    // empty wide string
  ACE_CDR::WChar *wstr = wchar2;
  ACE_CString str ("Test String");
  ACE_CDR::Short s = -123;
  ACE_CDR::UShort us =  123;
  ACE_CDR::Long l = -65800L;
  ACE_CDR::ULong ul =  65800UL;
  ACE_CDR::Float f =  1.23f;
  ACE_CDR::Double d =  123.456789;

  // Arrays for output
  ACE_CDR::Short s_array[3] = { -1, 0, 1 };
  ACE_CDR::Long l_array[3] = { -345678, 0, 345678 };
  ACE_CDR::Float f_array[3] = { -1.23f, 0.0f, 1.23f };
  ACE_CDR::Double d_array[3] = { -123.456789, 0.0, 123.456789 };
  ACE_CDR::Boolean b_array[3] = { false, true, false };

  ACE_OutputCDR::from_char fc (ch);
  ACE_OutputCDR::from_wchar fwc (wch);
  os << fc;
  os << fwc;
  os << str;
  os << wstr;
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
  os.write_boolean_array (b_array, 3);

  // Do the same for size stream.
  ss << fc;
  ss << fwc;
  ss << str;
  ss << wstr;
  ss << s;
  ss << us;
  ss << l;
  ss << ul;
  ss << f;
  ss << d;
  ss.write_short_array (s_array, 3);
  ss.write_long_array (l_array, 3);
  ss.write_float_array (f_array, 3);
  ss.write_double_array (d_array, 3);
  ss.write_boolean_array (b_array, 3);

  // Check the size.
  if (ss.total_length () != os.total_length ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("representation length does not match")),
                      1);

  const ACE_Message_Block *out_mb = os.begin ();
  size_t len = out_mb->length ();

  // Create an input stream (copy constructor)
  ACE_InputCDR is (os);
  const ACE_Message_Block *in_mb = is.start ();

  if (in_mb->length () != len)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("buffer length not preserved")),
                      1);

  for (i = 0; i < len; i++)
    {
      unsigned long const in_chunk =
        static_cast<unsigned long> (* (in_mb->rd_ptr () + i));

      unsigned long const out_chunk =
        static_cast<unsigned long> (* (out_mb->rd_ptr () + i));

      if (in_chunk != out_chunk)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("buffer contents not preserved")),
                          1);
    }

  // Basic types for input
  ACE_CDR::Char ch1 = '\0';
  ACE_CDR::WChar wch1 = '\x00';
  ACE_CDR::WChar *wstr1 = 0;
  ACE_CString str1;
  ACE_CDR::Short s1 = 0;
  ACE_CDR::UShort us1 = 0;
  ACE_CDR::Long l1 = 0L;
  ACE_CDR::ULong ul1 = 0UL;
  ACE_CDR::Float f1 = 0.0f;
  ACE_CDR::Double d1 = 0.0;

  // Arrays for input
  ACE_CDR::Short s_array1[3];
  ACE_CDR::Long l_array1[3];
  ACE_CDR::Float f_array1[3];
  ACE_CDR::Double d_array1[3];

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Checking operators and arrays\n\n")));

  ACE_InputCDR::to_char tc (ch1);
  is >> tc;
  ACE_InputCDR::to_wchar twc (wch1);
  is >> twc;
  is >> str1;
  ACE_InputCDR::to_wstring twstr (wstr1, 0);
  is >> twstr;
  // @todo Lose the ACE_Auto_Array_Ptr.  We should be using a
  //       std::string, or the like.
  ACE_Auto_Array_Ptr<ACE_CDR::WChar> safe_wstr (wstr1);
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
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("char transfer error")),
                      1);

  if (wch1 != wch)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("wchar transfer error")),
                      1);

  if (str1 != str)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("string transfer error")),
                      1);

  if (ACE_OS::wscmp (wstr1, wstr))
     ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("wide string transfer error")),
                       1);


  if (s1 != s)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("short transfer error")),
                      1);

  if (us1 != us)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("ushort transfer error")),
                      1);

  if (l1 != l)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("long transfer error")),
                      1);

  if (ul1 != ul)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("ulong transfer error")),
                      1);

  if (!ACE::is_equal (f1, f))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("float transfer error")),
                      1);

  if (!ACE::is_equal (d1, d))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("double transfer error")),
                      1);

  for (i = 0 ; i < 3; i++)
    if (s_array1[i] != s_array[i])
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("short array transfer error")),
                        1);

  for (i = 0 ; i < 3; i++)
    if (l_array1[i] != l_array[i])
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("long array transfer error")),
                        1);

  for (i = 0 ; i < 3; i++)
    if (!ACE::is_equal (f_array1[i], f_array[i]))
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("float array transfer error")),
                        1);

  for (i = 0 ; i < 3; i++)
    if (!ACE::is_equal (d_array1[i], d_array[i]))
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("double array transfer error")),
                        1);

  return 0;
}

int
CDR_Test_Types::test_put (ACE_OutputCDR &cdr)
{
  for (int i = 0; i < n; ++i)
    {
      if (cdr.write_octet (this->o) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("write_octet[%d] failed\n"),
                           i),
                          1);
      if (cdr.write_short (this->s) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("write_short[%d] failed\n"),
                           i),
                          1);
      if (cdr.write_octet (this->o) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("write_octet-2[%d] failed\n"),
                           i),
                          1);
      if (cdr.write_long (this->l) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("write_long[%d] failed\n"),
                           i),
                          1);
      if (cdr.write_long (this->l) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("write_long-2[%d] failed\n"),
                           i),
                          1);

      if (cdr.write_string (this->str) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("write_string[%d] failed\n"),
                           i),
                          1);

      if (cdr.write_wstring (this->wstr) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("write_wstring[%d] failed\n"),
                           i),
                          1);

    }

  return 0;
}

int
CDR_Test_Types::test_get (ACE_InputCDR &cdr) const
{
  ACE_CDR::Octet xo;
  ACE_CDR::Short xs;
  ACE_CDR::Long xl;

  for (int i = 0; i < n; ++i)
    {
      if (cdr.read_octet (xo) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("read_octet[%d] failed\n"),
                           i),
                          1);
      if (xo != this->o)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("octet[%d] differs\n"),
                           i),
                          1);
      if (cdr.read_short (xs) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("read_short[%d] failed\n"),
                           i), 1);
      if (xs != this->s)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("short[%d] differs\n"),
                           i),
                          1);
      if (cdr.read_octet (xo) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("read_octet-2[%d] failed\n"),
                           i),
                          1);
      if (xo != this->o)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("octet-2[%d] differs\n"),
                           i),
                          1);
      if (cdr.read_long (xl) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("read_long[%d] failed\n"),
                           i),
                          1);
      if (xl != this->l)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("long[%d] differs\n"),
                           i),
                          1);
      if (cdr.read_long (xl) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("read_long-2[%d] failed\n"),
                           i),
                          1);
      if (xl != this->l)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("long-2[%d] differs\n"),
                           i),
                          1);

      ACE_CDR::Char *xstr;
      if (cdr.read_string (xstr) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("read_string2[%d] failed\n"),
                           i),
                          1);
      ACE_Auto_Basic_Array_Ptr<ACE_CDR::Char> auto_xstr (xstr);
      if (ACE_OS::strcmp (auto_xstr.get (), this->str) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("string[%d] differs\n"),
                           i),
                          1);

      ACE_CDR::WChar *wstr1;
      if (cdr.read_wstring (wstr1) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT ("read_wstring2[%d] failed\n"),
                            i),
                           1);
      // zero length
      ACE_Auto_Basic_Array_Ptr<ACE_CDR::WChar> auto_xwstr (wstr1);
       if (ACE_OS::wslen(auto_xwstr.get () ))
         ACE_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT ("wstring[%d] differs\n"),
                             i),
                            1);
    }
  return 0;
}

int
CDR_Test_Types::test_put_placeholder (ACE_OutputCDR &cdr)
{
  // Write a placeholder then a bunch of other stuff, then replace.
  // Long
  char *pos = cdr.write_long_placeholder ();
  if (test_put (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_put (long placeholder) failed\n")),
                      1);
  if (!cdr.replace (this->repl, pos))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replace(long) failed\n")),
                      1);

  // ULong
  pos = cdr.write_long_placeholder ();
  if (test_put (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_put (long placeholder) failed\n")),
                      1);
  if (!cdr.replace (this->repul, pos))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replace(ulong) failed\n")),
                      1);

  // Short
  pos = cdr.write_short_placeholder ();
  if (test_put (cdr) != 0)
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("test_put (short placeholder) failed\n")),
                     1);
  if (!cdr.replace (this->reps, pos))
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("replace(short) failed\n")),
                     1);

  // UShort
  pos = cdr.write_short_placeholder ();
  if (test_put (cdr) != 0)
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("test_put (short placeholder) failed\n")),
                     1);
  if (!cdr.replace (this->repus, pos))
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("replace(ushort) failed\n")),
                     1);

  // Boolean
  pos = cdr.write_boolean_placeholder ();
  if (test_put (cdr) != 0)
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("test_put (boolean placeholder) failed\n")),
                     1);
  if (!cdr.replace (this->repb, pos))
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("replace(boolean) failed\n")),
                     1);

  // Char
  pos = cdr.write_char_placeholder ();
  if (test_put (cdr) != 0)
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("test_put (char placeholder) failed\n")),
                     1);
  if (!cdr.replace (this->repc, pos))
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("replace(char) failed\n")),
                     1);

  // LongLong
  pos = cdr.write_longlong_placeholder ();
  if (test_put (cdr) != 0)
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("test_put (longlong placeholder) failed\n")),
                     1);
  if (!cdr.replace (this->repll, pos))
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("replace(longlong) failed\n")),
                     1);

  // ULongLong
  pos = cdr.write_longlong_placeholder ();
  if (test_put (cdr) != 0)
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("test_put (longlong placeholder) failed\n")),
                     1);
  if (!cdr.replace (this->repull, pos))
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("replace(ulonglong) failed\n")),
                     1);

  // Octet
  pos = cdr.write_octet_placeholder ();
  if (test_put (cdr) != 0)
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("test_put (octet placeholder) failed\n")),
                     1);
  if (!cdr.replace (this->repo, pos))
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("replace(octet) failed\n")),
                     1);

  // Float
  pos = cdr.write_float_placeholder ();
  if (test_put (cdr) != 0)
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("test_put (float placeholder) failed\n")),
                     1);
  if (!cdr.replace (this->repf, pos))
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("replace(float) failed\n")),
                     1);

  // Double
  pos = cdr.write_double_placeholder ();
  if (test_put (cdr) != 0)
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("test_put (double placeholder) failed\n")),
                     1);
  if (!cdr.replace (this->repd, pos))
   ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("replace(double) failed\n")),
                     1);

  return 0;
}

int
CDR_Test_Types::test_get_placeholder (ACE_InputCDR &cdr) const
{
  ACE_CDR::Short xs;
  ACE_CDR::UShort xus;
  ACE_CDR::Long xl;
  ACE_CDR::ULong xul;
  ACE_CDR::Boolean xb;
  ACE_CDR::Char xc;
  ACE_CDR::LongLong xll;
  ACE_CDR::ULongLong xull;
  ACE_CDR::Octet xo;
  ACE_CDR::Float xf;
  ACE_CDR::Double xd;

  if (cdr.read_long (xl) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("read_long failed\n")),
                      1);
  if (xl != this->repl)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replaced long differs\n")),
                      1);

  // The bunch of stuff written after the placeholder by test_put_placeholder
  // should still be valid; check that it is.
  if (test_get (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_get (long) failed\n")),
                      1);

  if (cdr.read_ulong (xul) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("read_ulong failed\n")), 1);
  if (xul != this->repul)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replaced ulong differs\n")),
                      1);

  if (test_get (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_get (ulong) failed\n")),
                      1);

  if (cdr.read_short (xs) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("read_short failed\n")), 1);
  if (xs != this->reps)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replaced short differs\n")),
                      1);

  if (test_get (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_get (short) failed\n")),
                      1);

  if (cdr.read_ushort (xus) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("read_ushort failed\n")), 1);
  if (xus != this->repus)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replaced ushort differs\n")),
                      1);

  if (test_get (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_get (ushort) failed\n")),
                      1);

  if (cdr.read_boolean (xb) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("read_boolean failed\n")), 1);
  if (xb != this->repb)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replaced boolean differs\n")),
                      1);

  if (test_get (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_get (boolean) failed\n")),
                      1);

  if (cdr.read_char (xc) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("read_char failed\n")), 1);
  if (xc != this->repc)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replaced char differs\n")),
                      1);

  if (test_get (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_get (char) failed\n")),
                      1);


  if (cdr.read_longlong (xll) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("read_longlong failed\n")), 1);
  if (xll != this->repll)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replaced longlong differs\n")),
                      1);

  if (test_get (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_get (longlong) failed\n")),
                      1);

  if (cdr.read_ulonglong (xull) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("read_ulonglong failed\n")), 1);
  if (xull != this->repull)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replaced ulonglong differs\n")),
                      1);

  if (test_get (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_get (ulonglong) failed\n")),
                      1);

  if (cdr.read_octet (xo) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("read_octet failed\n")), 1);
  if (xo != this->repo)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replaced octet differs\n")),
                      1);

  if (test_get (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_get (octet) failed\n")),
                      1);

  if (cdr.read_float (xf) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("read_float failed\n")), 1);
  if (xf != this->repf)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replaced float differs\n")),
                      1);

  if (test_get (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_get (float) failed\n")),
                      1);

  if (cdr.read_double (xd) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("read_double failed\n")), 1);
  if (xd != this->repd)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("replaced double differs\n")),
                      1);

  if (test_get (cdr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test_get (double) failed\n")),
                      1);

  return 0;
}


int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("CDR_Test"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE::major_version (),
              ACE::minor_version(),
              ACE::beta_version()));

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("dn:l:"));
  int opt;
  int debug = 0;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
          case 'd':
            debug++;
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
                        ACE_TEXT ("Usage: %s ")
                        ACE_TEXT ("-d debug")
                        ACE_TEXT ("-n <num> ")
                        ACE_TEXT ("-l <loops> ")
                        ACE_TEXT ("\n"),
                        ACE_TEXT ("CDR_Test")));
            return -1;
        }
    }

  // Sanity checks.
  ACE_TEST_ASSERT (sizeof (ACE_CDR::Boolean)    >= 1);
  ACE_TEST_ASSERT (sizeof (ACE_CDR::Octet)      == 1);
  ACE_TEST_ASSERT (sizeof (ACE_CDR::WChar)      >= 2);
  ACE_TEST_ASSERT (sizeof (ACE_CDR::Short)      == 2);
  ACE_TEST_ASSERT (sizeof (ACE_CDR::Long)       == 4);
  ACE_TEST_ASSERT (sizeof (ACE_CDR::LongLong)   == 8);
  ACE_TEST_ASSERT (sizeof (ACE_CDR::Float)      == 4);
  ACE_TEST_ASSERT (sizeof (ACE_CDR::Double)     == 8);
  ACE_TEST_ASSERT (sizeof (ACE_CDR::LongDouble) == 16);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing ACE CDR functions - short stream\n\n")));

  if (short_stream () != 0 )
    return 1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Short stream - no errors\n\n")
              ACE_TEXT ("Testing basic types - long stream\n\n")));

  for (int i = 0; i < nloops; ++i)
    {
      ACE_OutputCDR output;
      CDR_Test_Types test_types;

      if (test_types.test_put (output) != 0)
        return 1;

      ACE_InputCDR input (output);
      if (debug > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Output CDR:\n")));
          ACE_HEX_DUMP ((LM_DEBUG,
                         input.rd_ptr(),
                         64));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Input CDR:\n")));
          ACE_HEX_DUMP ((LM_DEBUG,
                         input.rd_ptr(),
                         64));
        }

      if (test_types.test_get (input) != 0)
        return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Long stream - no errors\n\n")
              ACE_TEXT ("Testing basic types - long stream[2]\n\n")));

  for (int j = 0; j < nloops; ++j)
    {
      ACE_OutputCDR output;
      CDR_Test_Types test_types;

      if (test_types.test_put (output) != 0)
        return 1;

      ACE_InputCDR input (output.begin ());
      if (debug > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Output CDR:\n")));
          ACE_HEX_DUMP ((LM_DEBUG,
                         input.rd_ptr(),
                         64));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Input CDR:\n")));
          ACE_HEX_DUMP ((LM_DEBUG,
                         input.rd_ptr(),
                         64));
        }

      if (test_types.test_get (input) != 0)
        return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Long stream[2] - no errors\n\n")
              ACE_TEXT ("Testing consolidation\n\n")));

  ACE_OutputCDR output (30);
  CDR_Test_Types test_types;

  if (test_types.test_put (output) != 0)
    return 1;

  if (output.begin () == output.end ())
    ACE_DEBUG ((LM_WARNING,
                ACE_TEXT ("Only one block needed; test no effective.\n")));
  else
    {
      if (output.consolidate () != 0)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("consolidate")));
    }

  ACE_InputCDR input (output);
  if (debug > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Output CDR:\n")));
      ACE_HEX_DUMP ((LM_DEBUG,
                     input.rd_ptr(),
                     64));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Input CDR:\n")));
      ACE_HEX_DUMP ((LM_DEBUG,
                     input.rd_ptr(),
                     64));
    }

  if (test_types.test_get (input) != 0)
    return 1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Consolidation - no errors\n\n")
              ACE_TEXT ("Testing placeholder/replace\n\n")));

  output.reset();
  if (test_types.test_put_placeholder (output) != 0)
    return 1;

  input = ACE_InputCDR(output);
  if (debug > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Output CDR:\n")));
      ACE_HEX_DUMP ((LM_DEBUG,
                     input.rd_ptr(),
                     64));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Input CDR:\n")));
      ACE_HEX_DUMP ((LM_DEBUG,
                     input.rd_ptr(),
                     64));
    }

  if (test_types.test_get_placeholder (input) != 0)
    return 1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Placeholder/Replace - no errors\n\n")));

  ACE_END_TEST;
  return 0;
}

