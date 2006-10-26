// file      : Test/ExH/Converter/converter.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#include "Utility/ExH/Converter.hpp"
#include "Utility/ExH/StringStreamConverter.hpp"

#include <string>
#include <sstream>

using std::string;
using namespace Utility::ExH;

struct E {};

void postcondition (bool p) throw (E)
{
  if (!p) throw E ();
}

int
main ()
{
  try
  {
    // template<T>
    // converter (T const&)
    //
    {
      postcondition (converter ("hello") == string("hello"));
    }

    // template<>
    // converter (std::ostringstream const&)
    //
    {
      std::ostringstream ostr;
      ostr << "hello";
      postcondition (converter (ostr) == string("hello"));
    }
  }
  catch (...)
  {
    return -1;
  }
}
//$Id$
