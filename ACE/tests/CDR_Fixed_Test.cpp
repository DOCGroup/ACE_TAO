#include "test_config.h"
#include "ace/CDR_Base.h"
#include "ace/OS_NS_string.h"

#ifndef ACE_LACKS_IOSTREAM_TOTALLY
#include <fstream>
#endif

#define TEST_EQUAL(LHS, RHS) if (!((LHS) == (RHS))) {               \
  failed = true;                                                    \
  ACE_ERROR ((LM_ERROR, "Test assertion FAILED " #LHS " != " #RHS   \
              " at line %l\n"));                                    }

namespace
{
  bool failed = false;

  void log (ACE_OSTREAM_TYPE *os, const char *label, const char *expected,
            const ACE_CDR::Fixed &f)
  {
#ifdef ACE_LACKS_IOSTREAM_TOTALLY
    ACE_OS::fprintf (os, "%s: expected %s got ", label, expected);
#else
    *os << label << ": expected " << expected << " got ";
#endif
    *os << f;
#ifdef ACE_LACKS_IOSTREAM_TOTALLY
    ACE_OS::fputc ('\n', os);
#else
    *os << '\n';
#endif
  }
}

#define EXPECT(STR, OBJ)                                                \
{                                                                       \
  char buffer[Fixed::MAX_STRING_SIZE];                                  \
  OBJ.to_string (buffer, sizeof buffer);                                \
  if (ACE_OS::strcmp (STR, buffer)) {                                   \
    failed = true;                                                      \
    ACE_ERROR ((LM_ERROR, "FAILED conversion to string at line %l\n")); \
    log (os, #OBJ, STR, OBJ);                                           \
  }                                                                     \
}

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("CDR_Fixed_Test"));

  typedef ACE_CDR::Fixed Fixed;
  typedef ACE_CDR::LongLong LongLong;
  typedef ACE_CDR::ULongLong ULongLong;
  typedef ACE_CDR::LongDouble LongDouble;

  const Fixed f1 = Fixed::from_integer (LongLong (-1234567890l)),
    f2 = Fixed::from_integer (ULongLong (987654321ul)),
    f3 = Fixed::from_string ("612578912487901265.90125789");

  TEST_EQUAL (LongLong (-1234567890l), LongLong (f1));
  TEST_EQUAL (LongLong (987654321), LongLong (f2));
  TEST_EQUAL (LongLong (612578912487901265ull), LongLong (f3));

  TEST_EQUAL (0, f1.fixed_scale ());
  TEST_EQUAL (10, f1.fixed_digits ());
  TEST_EQUAL (0, f2.fixed_scale ());
  TEST_EQUAL (9, f2.fixed_digits ());
  TEST_EQUAL (8, f3.fixed_scale ());
  TEST_EQUAL (26, f3.fixed_digits ());

  ACE_OSTREAM_TYPE *const os = ace_file_stream::instance ()->output_file ();

  EXPECT ("-1234567890", f1);
  EXPECT ("987654321", f2);
  EXPECT ("612578912487901265.90125789", f3);

  Fixed f1_scaled = f1 / Fixed::from_integer (LongLong (100));
  const Fixed f4 = Fixed::from_string ("-12345678.9");
  TEST_EQUAL (f1_scaled, f4);

  EXPECT ("-12345678.9", f4);

  const Fixed f5 = Fixed::from_string ("0.00000000"),
              f6 = Fixed::from_string ("0.000000000");
  EXPECT ("0.00000000", f5);
  EXPECT ("0.000000000", f6);
  TEST_EQUAL (8, f5.fixed_scale ()); // these are not "normalized" since the
  TEST_EQUAL (9, f6.fixed_scale ()); // caller explicitly requested this scale

  Fixed f7 = Fixed::from_string ("0.1");
  EXPECT ("0", f7.round (0));
  EXPECT ("0", f7.truncate (0));

  f7 = Fixed::from_string ("0.05");
  EXPECT ("0.1", f7.round (1));
  EXPECT ("0.0", f7.truncate (1));

  f7 = Fixed::from_string ("-0.005");
  EXPECT ("-0.01", f7.round (2));
  EXPECT ("0.00", f7.truncate (2));

  LongDouble ld1;
  ACE_CDR_LONG_DOUBLE_ASSIGNMENT (ld1, 1234.45);
  const Fixed f8 = Fixed::from_floating (ld1);
  TEST_EQUAL (LongLong (1234), LongLong (f8));

  LongDouble ld2;
  ACE_CDR_LONG_DOUBLE_ASSIGNMENT (ld2, -0.3125);
  const Fixed f9 = Fixed::from_floating (ld2);
  EXPECT ("-0.3125", f9);

  LongDouble ld3;
  ACE_CDR_LONG_DOUBLE_ASSIGNMENT (ld3, 0.125);
  const Fixed f10 = Fixed::from_floating (ld3);
  EXPECT ("0.125", f10);

  LongDouble ldzero = ACE_CDR_LONG_DOUBLE_INITIALIZER;
  const Fixed fzero = Fixed::from_floating (ldzero);
  EXPECT ("0", fzero);
  ACE_CDR_LONG_DOUBLE_ASSIGNMENT(ldzero, 0);
  const Fixed fzero2 = Fixed::from_floating(ldzero);
  EXPECT("0", fzero2);

  Fixed f11 = Fixed::from_integer (LongLong (-1l));
  TEST_EQUAL (LongLong (-1), LongLong (f11));
  TEST_EQUAL (false, !f11);
  ++f11;
  TEST_EQUAL (true, !f11);
  TEST_EQUAL (LongLong (0), LongLong (f11));
  TEST_EQUAL (Fixed::from_integer (), f11);
  f11 += Fixed::from_string ("0.124357891");
  ++f11;
  TEST_EQUAL (Fixed::from_string ("1.124357891"), f11);
  --f11;
  --f11;
  TEST_EQUAL (Fixed::from_string ("-0.875642109"), f11);

  TEST_EQUAL (true, Fixed::from_integer (LongLong (-4)) < Fixed::from_integer (LongLong (2)));
  TEST_EQUAL (true, Fixed::from_integer (LongLong (-4)) < Fixed::from_integer (LongLong (-2)));
  TEST_EQUAL (false, Fixed::from_integer (LongLong (4)) < Fixed::from_integer (LongLong (-2)));
  TEST_EQUAL (false, Fixed::from_integer (LongLong (4)) < Fixed::from_integer (LongLong (2)));
  TEST_EQUAL (true, Fixed::from_string ("2.17") < Fixed::from_string ("3.142"));
  TEST_EQUAL (true, Fixed::from_string ("10.1") < Fixed::from_string ("100"));

  TEST_EQUAL (Fixed::from_string ("0000.000"), f5);
  TEST_EQUAL (Fixed::from_string ("-0.00500"), f7);
  TEST_EQUAL (f11, f11);

  Fixed f12 = f11++;
  TEST_EQUAL (++f12, f11);

  Fixed f13 = f12--;
  TEST_EQUAL (--f13, f12);

  Fixed f14 = Fixed::from_integer (LongLong (9));
  TEST_EQUAL (1, f14.fixed_digits ());
  ++f14;
  TEST_EQUAL (Fixed::from_integer (LongLong (10)), f14);

  TEST_EQUAL (Fixed::from_string ("778.33"),
              Fixed::from_string ("12.9") + Fixed::from_string ("765.43"));

  Fixed full = Fixed::from_string ("9876543210987654321098765432109"),
    before = full;
  full += Fixed::from_string ("0.99");
  TEST_EQUAL (before, full); // no rounding

  f14 += full;
  const Fixed f15 = Fixed::from_string ("9876543210987654321098765432119");
  TEST_EQUAL (f15, f14);

  Fixed f16 = Fixed::from_string ("123.4567890123456789012345678901");
  f16 += Fixed::from_integer (LongLong (9876));
  const Fixed f17 = Fixed::from_string ("9999.456789012345678901234567890");
  TEST_EQUAL (f17, f16);

  Fixed f18 = Fixed::from_string ("197854191");
  f18 -= Fixed::from_string ("123546789");
  EXPECT ("74307402", f18);
  TEST_EQUAL (8, f18.fixed_digits ());

  Fixed f19 = Fixed::from_integer (LongLong (9));
  f19 -= Fixed::from_integer (LongLong (10));
  EXPECT ("-1", f19);

  Fixed f20 = Fixed::from_integer (LongLong (99));
  f20 += Fixed::from_integer (LongLong (99));
  EXPECT ("198", f20); // carry extra digit

  Fixed f21 = Fixed::from_string ("7.532");
  f21 -= Fixed::from_string ("4.91");
  EXPECT ("2.622", f21);

  Fixed f22 = Fixed::from_integer (LongLong (-99)) * Fixed::from_integer (LongLong (-9));
  EXPECT ("891", f22);

  Fixed f23 = Fixed::from_integer (LongLong (9)) * Fixed::from_integer (LongLong (-99));
  EXPECT ("-891", f23);

  Fixed f24 = Fixed::from_string ("-3.4") * Fixed::from_string ("5.67");
  EXPECT ("-19.278", f24);

  Fixed f25 = Fixed::from_string ("9999999999.1234567890") // 10.10
    * Fixed::from_string ("876543219087654321.9876543210");// 18.10
        EXPECT ("8765432190108215212037174200.146", f25);  // 28.3

  //                                 1234567890123456789012345678901
  Fixed f26 = Fixed::from_string ("0.0000000000000000000000000000001")
            * Fixed::from_string ("0.1");
  EXPECT (                        "0.0000000000000000000000000000000", f26);

  Fixed f27 = Fixed::from_string ("817459124");
  f27 /= Fixed::from_string ("0.001");
  EXPECT ("817459124000", f27);
  f27 /= Fixed::from_integer (LongLong (1000));
  EXPECT ("817459124", f27);
  f27 /= Fixed::from_integer (LongLong (-2));
  EXPECT ("-408729562", f27);

  Fixed f28 = Fixed::from_string ("200000000");
  f28 /= Fixed::from_string ("500");
  EXPECT ("400000", f28);
  TEST_EQUAL (6, f28.fixed_digits ());

  Fixed f29 = Fixed::from_integer (LongLong (1)) / Fixed::from_integer (LongLong (-3));
  EXPECT ("-0.333333333333333333333333333333", f29); //TODO: one more 3

  Fixed f30 = Fixed::from_string("-9999752.0000") / Fixed::from_string("-4999876.00");
  EXPECT ("2", f30);

  ACE_END_TEST;
  return failed;
}
