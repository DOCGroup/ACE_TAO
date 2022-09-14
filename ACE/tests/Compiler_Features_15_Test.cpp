/**
 * This program checks if the compiler / platform supports default
 * constructor and assignment operators
 */

#include "test_config.h"

class A
{
public:
  A ();
  A (A&&) = default;
  A& operator= (A&&) = default;
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_15_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Defaulted move constructor and assignment operators work.\n")));

  ACE_END_TEST;

  return 0;
}
