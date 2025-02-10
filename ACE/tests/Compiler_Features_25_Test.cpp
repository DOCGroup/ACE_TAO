/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

#include <type_traits>

struct Foo
{
  using is_true = std::true_type;
  using is_false = std::false_type;
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_25_Test"));

  int retval = 0;
  if (Foo::is_true() && !Foo::is_false())
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Compiler Feature 25 Test does compile and run.\n")));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ERROR: Compiler Feature 25 Test does not run correctly.\n")));
    }

  ACE_END_TEST;

  return retval;
}
