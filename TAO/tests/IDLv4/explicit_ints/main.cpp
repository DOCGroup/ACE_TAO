#include "testC.h"

#include "ace/OS_main.h"
#include "ace/streams.h"

namespace stream8 {
  std::ostream &
  operator<< (std::ostream &os, CORBA::Uint8 value) {
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
  const bool failed = actual != expected;
  if (failed)
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

  expect_equals<CORBA::Uint8>(any_failed, "u8_max", u8_max, 255);
  expect_equals<CORBA::Int8>(any_failed, "i8_min", i8_min, -128);
  expect_equals<CORBA::Int8>(any_failed, "i8_max", i8_max, 127);
  expect_equals<CORBA::Uint16>(any_failed, "u16_max", u16_max, 65535);
  expect_equals<CORBA::Int16>(any_failed, "i16_min", i16_min, -32768);
  expect_equals<CORBA::Int16>(any_failed, "i16_max", i16_max, 32767);
  expect_equals<CORBA::Uint32>(any_failed, "u32_max", u32_max, 4294967295);
  expect_equals<CORBA::Int32>(any_failed, "i32_min", i32_min, -2147483648);
  expect_equals<CORBA::Int32>(any_failed, "i32_max", i32_max, 2147483647);
  expect_equals<CORBA::Uint64>(any_failed, "u64_max", u64_max, 18446744073709551615ULL);
  expect_equals<CORBA::Int64>(any_failed, "i64_min", i64_min, (-9223372036854775807 - 1));
  expect_equals<CORBA::Int64>(any_failed, "i64_max", i64_max, 9223372036854775807);

  expect_equals<CORBA::Uint8>(any_failed, "u8_max_overflow", u8_max_overflow, 0);
  expect_equals<CORBA::Int8>(any_failed, "i8_max_overflow", i8_max_overflow, i8_min);

  expect_equals<CORBA::Uint8>(any_failed, "u8_max_negate", u8_max_negate, 0);
  expect_equals<CORBA::Int8>(any_failed, "i8_max_negate", i8_max_negate, i8_min);

  return any_failed ? 1 : 0;
}
