#include "testC.h"

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_main.h"
#include "ace/streams.h"
#include <cstdlib>

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

int ACE_TMAIN(int, ACE_TCHAR *[])
{
  DataStruct d;
  d.mapData[0] = 1;

  bool any_failed = false;
  expect_equals<CORBA::Long> (any_failed, "d.mapData.size()", d.mapData.size(), 1);

  return EXIT_SUCCESS;
}
