// $Id$

/**
 * This program checks if the compiler / platform supports constexpr
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <limits>
#include <stdint.h>

class A
{
public:
  A ();
  static constexpr char const* adapter_name = "TAOX11_AnyInsert_Adapter";
  static constexpr uint32_t bound = std::numeric_limits<uint32_t>::max();
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_17_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Constexpr works.\n")));

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_17_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
