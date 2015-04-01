#include "test_config.h"
#include "ace/CDR_Base.h"

#ifndef ACE_LACKS_IOSTREAM_TOTALLY
#include <fstream>
#endif

namespace
{
  bool failed = false;
}

#define TEST_EQUAL(lhs, rhs) if (!(lhs == rhs)) {                   \
  failed = true;                                                    \
  ACE_ERROR ((LM_ERROR, "Test assertion FAILED " #lhs " != " #rhs   \
              " at line %l\n"));                                    }

namespace
{
  void newline (ACE_OSTREAM_TYPE* os)
  {
#ifdef ACE_LACKS_IOSTREAM_TOTALLY
    ACE_OS::fprintf (os, "\n");
#else
    *os << '\n';
#endif
  }
}

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("CDR_Fixed_Test"));

  typedef ACE_CDR::Fixed Fixed;

  const Fixed f1 = Fixed::from_int (1234567890l),
    f2 = Fixed::from_int (987654321ul),
    f3 = Fixed::from_string ("612578912487901265.90125789");

  TEST_EQUAL (0, f1.scale ());
  TEST_EQUAL (0, f2.scale ());
  TEST_EQUAL (8, f3.scale ());

  Fixed f1_scaled = f1;
  f1_scaled.scale () = 2;
  const Fixed f4 = Fixed::from_string ("12345678.9");
  TEST_EQUAL (f1_scaled, f4);

  *ace_file_stream::instance ()->output_file () << f1_scaled;
  newline (ace_file_stream::instance ()->output_file ());
  *ace_file_stream::instance ()->output_file () << f2;
  newline (ace_file_stream::instance ()->output_file ());
  *ace_file_stream::instance ()->output_file () << f3;
  newline (ace_file_stream::instance ()->output_file ());
  *ace_file_stream::instance ()->output_file () << f4;
  newline (ace_file_stream::instance ()->output_file ());

  const Fixed f5 = Fixed::from_string ("0.00000000"),
    f6 = Fixed::from_string ("0.000000000");
  *ace_file_stream::instance ()->output_file () << f5;
  newline (ace_file_stream::instance ()->output_file ());
  *ace_file_stream::instance ()->output_file () << f6;
  newline (ace_file_stream::instance ()->output_file ());

  TEST_EQUAL (8, f5.scale ());
  TEST_EQUAL (9, f6.scale ());

  Fixed f7 = Fixed::from_int (0l);
  *ace_file_stream::instance ()->output_file () << f7;
  newline (ace_file_stream::instance ()->output_file ());
  f7.scale () = 3;
  *ace_file_stream::instance ()->output_file () << f7;
  newline (ace_file_stream::instance ()->output_file ());
  f7.scale () = 4;
  *ace_file_stream::instance ()->output_file () << f7;
  newline (ace_file_stream::instance ()->output_file ());

  ACE_END_TEST;
  return failed;
}
