#include "testC.h"

#include "tao/idl_features.h"
#ifdef TAO_IDL_HAS_EXPLICIT_INTS
#  if !TAO_IDL_HAS_EXPLICIT_INTS
#    error "Expecting macro to be true"
#  endif
#else
#  error "Expecting macro to be defined"
#endif

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_main.h"
#include "ace/streams.h"

namespace stream8 {
  std::ostream &
  operator<< (std::ostream &os, CORBA::UInt8 value) {
    return os << static_cast<unsigned>(value);
  }

  std::ostream &
  operator<< (std::ostream &os, CORBA::Int8 value) {
    return os << static_cast<int>(value);
  }
}

template <typename IntType>
void
expect_equals (bool &any_failed, const char *name, IntType actual, IntType expected)
{
  if (actual != expected)
    {
      using stream8::operator<<;
      *ACE_DEFAULT_LOG_STREAM
        << "ERROR: For " << name << " expected: " << expected
        << ", but got " << actual << "\n";
      any_failed = true;
    }
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  bool any_failed = false;

  expect_equals<CORBA::UInt8> (any_failed, "u8_max", u8_max, 255);
  expect_equals<CORBA::Int8> (any_failed, "i8_min", i8_min, -128);
  expect_equals<CORBA::Int8> (any_failed, "i8_max", i8_max, 127);
  expect_equals<CORBA::UInt16> (any_failed, "u16_max", u16_max, 65535);
  expect_equals<CORBA::Int16> (any_failed, "i16_min", i16_min, -32768);
  expect_equals<CORBA::Int16> (any_failed, "i16_max", i16_max, 32767);
  expect_equals<CORBA::UInt32> (any_failed, "u32_max", u32_max, 4294967295);
  expect_equals<CORBA::Int32> (any_failed, "i32_min", i32_min, -2147483647 - 1);
  expect_equals<CORBA::Int32> (any_failed, "i32_max", i32_max, 2147483647);
  expect_equals<CORBA::UInt64> (any_failed, "u64_max", u64_max, 18446744073709551615ULL);
  expect_equals<CORBA::Int64> (any_failed, "i64_min", i64_min, -9223372036854775807 - 1);
  expect_equals<CORBA::Int64> (any_failed, "i64_max", i64_max, 9223372036854775807);

  expect_equals<CORBA::UInt8> (any_failed, "u8_min_overflow", u8_min_overflow, u8_max);
  expect_equals<CORBA::Int8> (any_failed, "i8_min_overflow", i8_min_overflow, i8_max);
  expect_equals<CORBA::UInt8> (any_failed, "u8_max_overflow", u8_max_overflow, 0);
  expect_equals<CORBA::Int8> (any_failed, "i8_max_overflow", i8_max_overflow, i8_min);

  expect_equals<CORBA::UInt8> (any_failed, "u8_max_negate", u8_max_negate, 0);
  expect_equals<CORBA::Int8> (any_failed, "i8_max_negate", i8_max_negate, i8_min);

  expect_equals<CORBA::UInt8> (any_failed, "u8_e1", u8_e1, 2);
  expect_equals<CORBA::UInt8> (any_failed, "u8_e2", u8_e2, 4);
  expect_equals<CORBA::UInt8> (any_failed, "u8_e3", u8_e3, 12);
  expect_equals<CORBA::UInt8> (any_failed, "u8_e4", u8_e4, 3);
  expect_equals<CORBA::UInt8> (any_failed, "u8_e5", u8_e5, 7);
  expect_equals<CORBA::UInt8> (any_failed, "u8_e6", u8_e6, 1);
  expect_equals<CORBA::UInt8> (any_failed, "u8_e7", u8_e7, 1);
  expect_equals<CORBA::UInt8> (any_failed, "u8_e8", u8_e8, 16);
  expect_equals<CORBA::UInt8> (any_failed, "u8_e9", u8_e9, 8);

  expect_equals<CORBA::Int8> (any_failed, "i8_e1", i8_e1, -2);
  expect_equals<CORBA::Int8> (any_failed, "i8_e2", i8_e2, 4);
  expect_equals<CORBA::Int8> (any_failed, "i8_e3", i8_e3, 12);
  expect_equals<CORBA::Int8> (any_failed, "i8_e4", i8_e4, 3);
  expect_equals<CORBA::Int8> (any_failed, "i8_e5", i8_e5, 7);
  expect_equals<CORBA::Int8> (any_failed, "i8_e6", i8_e6, 1);
  expect_equals<CORBA::Int8> (any_failed, "i8_e7", i8_e7, 1);
  expect_equals<CORBA::Int8> (any_failed, "i8_e8", i8_e8, 16);
  expect_equals<CORBA::Int8> (any_failed, "i8_e9", i8_e9, 8);

  return any_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
