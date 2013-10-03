// $Id$

/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <type_traits>

struct Foo
{
  typedef std::true_type is_true;
  typedef std::false_type is_false;
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

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_25_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
