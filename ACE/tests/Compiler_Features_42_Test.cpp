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

    inline void swap (unionarrayofstring& u);

  private:
    inline void _swap_u (unionarrayofstring& u);
    inline void _move_u (unionarrayofstring& u);
    inline void _clear ();

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
  this->_clear ();
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
  this->_move_u (u);
}

inline ::Test::unionarrayofstring& Test::unionarrayofstring::operator= (const ::Test::unionarrayofstring& u)
{
  if (this != std::addressof (u))
  {
    unionarrayofstring tmp (u);
    this->swap (tmp);
  }
  return *this;
}

inline ::Test::unionarrayofstring& Test::unionarrayofstring::operator= (::Test::unionarrayofstring&& u)
{
  if (this != std::addressof (u))
  {
    unionarrayofstring tmp (std::move (u));
    this->swap (tmp);
  }
  return *this;
}

inline void Test::unionarrayofstring::swap (::Test::unionarrayofstring& u)
{
  if (this != std::addressof (u))
  {
    if (this->disc_ != u.disc_)
    {
      ::Test::unionarrayofstring intermediate (std::move (*this));
      this->disc_ = std::move (u.disc_);
      this->_move_u (u);
      u.disc_ = std::move (intermediate.disc_);
      u._move_u (intermediate);
    }
    else
    {
      this->_swap_u (u);
    }
  }
}

inline void Test::unionarrayofstring::_swap_u (::Test::unionarrayofstring& u)
{
  switch (this->disc_)
  {
    case 1:
    {
      std::swap (this->u_.b_, u.u_.b_);
    }
    break;
    default:
    {
      std::swap (this->u_.as_, u.u_.as_);
    }
    break;
  }
}

inline void Test::unionarrayofstring::_move_u (::Test::unionarrayofstring& u)
{
  switch (this->disc_)
  {
    case 1:
    {
      this->u_.b_ = std::move (u.u_.b_);
    }
    break;
    default:
    {
      new (std::addressof (this->u_.as_)) arrayofstring (std::move (u.u_.as_));
    }
    break;
  }
}

inline void Test::unionarrayofstring::_clear ()
{
  switch (this->disc_)
  {
    case 1:
      break;
    default:
    {
      this->u_.as_.arrayofstring::~arrayofstring ();
    }
    break;
  }
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_42_Test"));

  ::Test::unionarrayofstring* p = new (::Test::unionarrayofstring);
  delete p;

  ACE_END_TEST;

  return 0;
}
