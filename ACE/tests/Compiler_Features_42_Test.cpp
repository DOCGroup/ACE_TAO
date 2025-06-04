/**
 * This program reproduces an compiler issue with Visual Studio 2022
 */

#include "test_config.h"
#include <array>
#include <string>

namespace Test
{
  using arrayofstring = std::array<std::string, 5>;

  class unionarrayofstring
  {
  public:
    unionarrayofstring () = default;
    unionarrayofstring (const unionarrayofstring&) = delete;
    unionarrayofstring (unionarrayofstring&&) = delete;
    inline ~unionarrayofstring () {}
    unionarrayofstring& operator= (const unionarrayofstring&) = delete;
    unionarrayofstring& operator= (unionarrayofstring&&) = delete;

  private:
    union u_type_
    {
      u_type_ () = default;
      ~u_type_ () {}
      ::Test::arrayofstring as_{};
    } u_{};
  };
}

int run_main (int, ACE_TCHAR*[])
{
  ACE_START_TEST (ACE_TEXT ("Compiler_Features_42_Test"));

  ::Test::unionarrayofstring p;

  ACE_END_TEST;

  return 0;
}
