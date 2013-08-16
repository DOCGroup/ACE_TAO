// $Id$

/**
 * This program checks if the compiler / platform supports strongly
 * array
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <array>
namespace Test
{
  typedef std::array< float, 5 > arrayType;
  typedef std::array< arrayType, 5 > array_array;
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_18_Test"));

  Test::array_array v_in = { { Test::arrayType {
    { 1.0, 2.0, 3.0, 4.0, 5.0 } }, Test::arrayType { { 1.0, 2.0, 3.0, 4.0,
    5.0 } }, Test::arrayType { { 1.0, 2.0, 3.0, 4.0, 5.0 } },
    Test::arrayType { { 1.0, 2.0, 3.0, 4.0, 5.0 } }, Test::arrayType { {
    1.0, 2.0, 3.0, 4.0, 5.0 } } } };

  ACE_UNUSED_ARG (v_in);

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Std arrays work.\n")));

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_18_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
