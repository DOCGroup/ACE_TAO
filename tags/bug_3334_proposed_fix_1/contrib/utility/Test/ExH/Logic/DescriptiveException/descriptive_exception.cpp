// file      : Test/ExH/Logic/DescriptiveException/descriptive_exception.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

/* FUZZ: disable check_for_improper_main_declaration */

#include "Utility/ExH/Logic/DescriptiveException.hpp"

#include <string>

using std::string;
using namespace Utility::ExH::Logic;

struct E {};

void postcondition (bool p)
{
  if (!p) throw E ();
}

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

  string str_;
};

int
main ()
{
  try
  {
    // DescriptiveException (char const*)
    //
    {
      DescriptiveException a ("hello");

      postcondition (a.what () == string ("hello"));
    }

    // DescriptiveException (std::string const&)
    //
    {
      DescriptiveException a (string ("hello"));

      postcondition (a.what () == string ("hello"));
    }


    // DescriptiveException (T const&)
    //
    {
      StringHolder a ("hello");

      DescriptiveException b (a);

      postcondition (b.what () == string ("hello"));
    }

    // DescriptiveException (DescriptiveException const&)
    //
    {
      DescriptiveException a ("hello");
      DescriptiveException b (a);

      postcondition (b.what () == string ("hello"));
    }

    // ~DescriptiveException
    //

    // operator= (DescriptiveException const&)
    //
    {
      DescriptiveException a ("hello");
      DescriptiveException b ("foo");
      b = a;

      postcondition (b.what () == string ("hello"));
    }

    // DescriptiveException ()
    //

    // init (char const*)
    //

    // what ()
    //
    {
      DescriptiveException a ("hello");

      postcondition (a.what () == string ("hello"));
    }
  }
  catch (...)
  {
    return -1;
  }
}
//$Id$
