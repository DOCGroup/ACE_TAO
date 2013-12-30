// $Id$

/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <cstdint>

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_30_Test"));

  int8_t t[] = {};
  int8_t x[] {};
  ACE_UNUSED_ARG (t);
  ACE_UNUSED_ARG (x);

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("C++11 support ok\n")));

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_30_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
