#include "test_config.h"
#include "ace/CDR_Base.h"

#ifndef ACE_LACKS_IOSTREAM_TOTALLY
#include <fstream>
#endif

#define TEST_EQUAL(lhs, rhs) if (!((lhs) == (rhs))) {               \
  failed = true;                                                    \
  ACE_ERROR ((LM_ERROR, "Test assertion FAILED " #lhs " != " #rhs   \
              " at line %l\n"));                                    }

namespace
{
  bool failed = false;

  void newline (ACE_OSTREAM_TYPE *os)
  {
#ifdef ACE_LACKS_IOSTREAM_TOTALLY
    ACE_OS::fputc ('\n', os);
#else
    *os << '\n';
#endif
  }

  struct newline_t {} nl;
  ACE_OSTREAM_TYPE &operator<< (ACE_OSTREAM_TYPE &lhs, const newline_t &)
  {
    newline (&lhs);
    return lhs;
  }
}

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("CDR_Fixed_Test"));

  typedef ACE_CDR::Fixed Fixed;

  const Fixed f1 = Fixed::from_integer (-1234567890l),
    f2 = Fixed::from_integer (987654321ul),
    f3 = Fixed::from_string ("612578912487901265.90125789");

  TEST_EQUAL (ACE_CDR::LongLong (-1234567890l), ACE_CDR::LongLong (f1));
  TEST_EQUAL (ACE_CDR::LongLong (987654321), ACE_CDR::LongLong (f2));
  TEST_EQUAL (ACE_CDR::LongLong (612578912487901265ul), ACE_CDR::LongLong (f3));

  TEST_EQUAL (0, f1.fixed_scale ());
  TEST_EQUAL (0, f2.fixed_scale ());
  TEST_EQUAL (8, f3.fixed_scale ());

  Fixed f1_scaled = f1 / Fixed::from_integer (100l);
  const Fixed f4 = Fixed::from_string ("-12345678.9");
  //TODO: division not yet implemented  TEST_EQUAL (f1_scaled, f4);

  ACE_OSTREAM_TYPE *const os = ace_file_stream::instance ()->output_file ();

  *os << f1_scaled << nl;
  *os << f2 << nl;
  *os << f3 << nl;
  *os << f4 << nl;

  const Fixed f5 = Fixed::from_string ("0.00000000"),
              f6 = Fixed::from_string ("0.000000000");
  *os << f5 << nl;
  *os << f6 << nl;

  TEST_EQUAL (8, f5.fixed_scale ()); // these are not "normalized" since the
  TEST_EQUAL (9, f6.fixed_scale ()); // caller explicitly requested this scale

  Fixed f7 = Fixed::from_string ("0.1");
  *os << f7.round (0) << nl;
  *os << f7.truncate (0) << nl;
  f7 = Fixed::from_string ("0.05");
  *os << f7.round (1) << nl;
  *os << f7.truncate (1) << nl;
  f7 = Fixed::from_string ("-0.005");
  *os << f7.round (2) << nl;
  *os << f7.truncate (2) << nl;

  const Fixed f8 = Fixed::from_floating (1234.45l);
  TEST_EQUAL (ACE_CDR::LongLong (1234), ACE_CDR::LongLong (f8));
  TEST_EQUAL (ACE_CDR::LongDouble (1234.45l), ACE_CDR::LongDouble (f8));
  *os << f8 << nl;

  const Fixed f9 = Fixed::from_floating (-0.3125l);
  *os << f9 << nl;

  const Fixed f10 = Fixed::from_floating (0.125l);
  *os << f10 << nl;

  Fixed f11 = Fixed::from_integer (-1l);
  TEST_EQUAL (ACE_CDR::LongLong (-1), ACE_CDR::LongLong (f11));
  ++f11;
  TEST_EQUAL (ACE_CDR::LongLong (0), ACE_CDR::LongLong (f11));
  TEST_EQUAL (Fixed::from_integer (0l), f11);

  TEST_EQUAL (true, Fixed::from_integer (-4l) < Fixed::from_integer (2l));
  TEST_EQUAL (true, Fixed::from_integer (-4l) < Fixed::from_integer (-2l));
  TEST_EQUAL (false, Fixed::from_integer (4l) < Fixed::from_integer (-2l));
  TEST_EQUAL (false, Fixed::from_integer (4l) < Fixed::from_integer (2l));
  TEST_EQUAL (true, Fixed::from_string ("2.17") < Fixed::from_string ("3.142"));
  TEST_EQUAL (true, Fixed::from_string ("10.1") < Fixed::from_string ("100"));

  ACE_END_TEST;
  return failed;
}
