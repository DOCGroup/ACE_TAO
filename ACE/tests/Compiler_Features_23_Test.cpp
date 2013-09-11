// $Id$

/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <array>
#include <string>

std::array<std::string, 2> priority_names =
{
  {
     std::string("NOT USED"),
     "P_PANIC"
  }
};

std::string t (std::uint32_t prop)
{
  std::uint32_t log = 0;

  for (; prop > 1; ++log)
    prop >>= 1;

  return priority_names[log];
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_23_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Compiler Feature 23 Test does compile and run.\n")));

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_23_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
