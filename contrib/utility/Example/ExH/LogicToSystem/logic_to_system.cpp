// file      : Example/ExH/LogicToSystem/logic_to_system.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

/* FUZZ: disable check_for_improper_main_declaration */

#include "Utility/ExH/System/Exception.hpp"
#include "Utility/ExH/Logic/Exception.hpp"

#include <iostream>

using std::cerr;
using std::endl;


struct SubsystemA
{
  class Exception : public Utility::ExH::Logic::Exception {};

  void
  foo ()
  {
    throw Exception ();
  }
};


struct SubsystemB
{
  void
  foo ()
  {
    SubsystemA a;
    a.foo ();

    // Here SubsystemB is using SunsystemA but cannot (forgot, doesnt't
    // want to, doesn't know how to, etc - pick your favorite) handle
    // exception thrown by SubsystemA. As a result exception is
    // 'converted' to System::Exception.
  }
};


int
main ()
{
  try
  {
    SubsystemB b;
    b.foo ();
  }
  catch (Utility::ExH::System::Exception const& ex)
  {
    cerr << "Caught Utility::ExH::System::Exception: "
         << ex.what ()
         << endl;
  }
}
//$Id$
