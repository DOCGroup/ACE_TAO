// $Id$

/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <type_traits>

class T_base;

template<typename T,
         typename = typename std::enable_if<
           std::is_base_of<T_base, T>::value>::type, typename ...Args>
void f(T const&);

template<typename T,
         typename = typename std::enable_if<
           std::is_base_of<T_base, T>::value>::type, typename ...Args>
inline void f(T const&)
{
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_24_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Compiler Feature 24 Test does compile and run.\n")));

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_24_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
