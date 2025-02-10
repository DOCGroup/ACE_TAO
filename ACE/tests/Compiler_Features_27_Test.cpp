/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

#include <string>

class Foo
{
public:
  Foo () = default;
};

void operator <<= (Foo&, std::string &&)
{
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_27_Test"));

  Foo any;
  any <<= std::move("abc");

  ACE_END_TEST;

  return 0;
}
