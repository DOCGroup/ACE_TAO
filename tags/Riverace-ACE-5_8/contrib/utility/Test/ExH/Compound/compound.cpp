// file      : Test/ExH/Compound/compound.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#include "Utility/ExH/Compound.hpp"

#include <string>

using namespace Utility::ExH;

struct E {};

void postcondition (bool p)
{
  if (!p) throw E ();
}

class Base
{
protected:
  Base () throw ()
  {
  }

  void
  init (char const* description) throw ()
  {
    str_ = description;
  }

  char const*
  what () const throw ()
  {
    return str_.c_str ();
  }

public:
  std::string str_;
};

class A_ {};
typedef
Compound<A_, Base>
A;

struct StringHolder
{
  StringHolder (char const* s)
      : str_ (s)
  {
  }

  operator std::string () const
  {
    return str_;
  }

  std::string str_;
};


int main ()
{
  try
  {
    // Compound (char const*)
    //
    {
      A a ("hello");

      postcondition (a.str_ == "hello");
    }

    // Compound (T const&)
    //
    {
      StringHolder a ("hello");
      A b (a);

      postcondition (b.str_ == "hello");
    }

    // Compound (Compound const&)
    //
    {
      A a ("hello");
      A b (a);

      postcondition (b.str_ == "hello");
    }

    // ~Compound ()
    //

    // operator= (Compound const&)
    //
    {
      A a ("hello");
      A b ("foo");
      b = a;

      postcondition (b.str_ == "hello");
    }

    // Compound ()
    //

  }
  catch (...)
  {
    return -1;
  }
}
//$Id$
