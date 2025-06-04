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
  inline unionarrayofstring (const unionarrayofstring&);
  inline unionarrayofstring (unionarrayofstring&&);
  inline ~unionarrayofstring ();
  inline unionarrayofstring& operator= (const unionarrayofstring&);
  inline unionarrayofstring& operator= (unionarrayofstring&&);

private:
  int32_t disc_{ (-2147483647 - 1) };
  union u_type_
  {
    u_type_ () = default;
    ~u_type_ ();
    ::Test::arrayofstring as_{};
    int32_t b_;
  } u_{};
};
}

inline Test::unionarrayofstring::u_type_::~u_type_ ()
{}

inline Test::unionarrayofstring::~unionarrayofstring ()
{
}

inline Test::unionarrayofstring::unionarrayofstring (const ::Test::unionarrayofstring& u)
  : disc_ (u.disc_)
{
  switch (this->disc_)
  {
    case 1:
    {
      this->u_.b_ = u.u_.b_;
    }
    break;
    default:
    {
      new (std::addressof (this->u_.as_)) arrayofstring (u.u_.as_);
    }
    break;
  }
}

inline Test::unionarrayofstring::unionarrayofstring (::Test::unionarrayofstring&& u)
  : disc_ (std::move (u.disc_))
{
}

inline ::Test::unionarrayofstring& Test::unionarrayofstring::operator= (const ::Test::unionarrayofstring& u)
{
  if (this != std::addressof (u))
  {
    unionarrayofstring tmp (u);
  }
  return *this;
}

inline ::Test::unionarrayofstring& Test::unionarrayofstring::operator= (::Test::unionarrayofstring&& u)
{
  if (this != std::addressof (u))
  {
    unionarrayofstring tmp (std::move (u));
  }
  return *this;
}

int run_main (int, ACE_TCHAR*[])
{
  ACE_START_TEST (ACE_TEXT ("Compiler_Features_42_Test"));

  ::Test::unionarrayofstring* p = new (::Test::unionarrayofstring);
  delete p;

  ACE_END_TEST;

  return 0;
}
