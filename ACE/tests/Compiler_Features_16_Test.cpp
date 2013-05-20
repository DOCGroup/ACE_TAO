// $Id$

/**
 * This program checks if the compiler / platform supports strongly
 * typed enums
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <limits>
#include <stdint.h>
#include <ostream>

static constexpr uint32_t bound = std::numeric_limits<uint32_t>::max();

namespace CORBA {
  // First forward declare TCKind, this is legal with C++11
  enum class TCKind : uint32_t;

  enum class TCKind : uint32_t
  {
    tk_null,
    tk_void,
    tk_event
  };// TCKind
};

std::ostream& operator<<
(std::ostream& strm,CORBA::TCKind _enumerator)
{
  switch (_enumerator) {
    case CORBA::TCKind::tk_null: return strm << "CORBA::TCKind::tk_null"; break;
    case CORBA::TCKind::tk_void: return strm << "CORBA::TCKind::tk_void"; break;
    default: return strm;
  }
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_16_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Strongly typed enums work.\n")));

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_16_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
