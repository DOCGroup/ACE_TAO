/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <type_traits>

template <typename T>
struct non_instantiatable
{
  typedef typename T::THIS_TYPE_CANNOT_BE_INSTANTIATED type;
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_36_Test"));

  bool const result = std::is_base_of<non_instantiatable<int>, void>::value;
  ACE_UNUSED_ARG (result);

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("C++11 support ok\n")));

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_36_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
