/**
 * This program checks if the compiler / platform supports strongly
 * array
 */

#include "test_config.h"

#include <array>
namespace Test
{
  using arrayType = std::array<float, 5>;
  using array_array = std::array<arrayType, 5>;
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
