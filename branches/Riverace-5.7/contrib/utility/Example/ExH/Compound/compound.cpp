// file      : Example/ExH/Compound/compound.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

/* FUZZ: disable check_for_improper_main_declaration */

#include "Utility/ExH/Compound.hpp"
#include "Utility/ExH/System/Exception.hpp"
#include "Utility/ExH/Logic/DescriptiveException.hpp"

// Include some helper converters to allow exception initialization
// with std::ostringstream

#include "Utility/ExH/StringStreamConverter.hpp"

#include <iostream>

using std::cerr;
using std::endl;
using std::string;
using std::ostringstream;

using namespace Utility::ExH;

// Here are our components

class Base
{
public:

  //
  // Exception definitions.
  //

  // Base logic exception class for component.
  class Exception_ {};
  typedef
  Compound <Exception_, Logic::DescriptiveException>
  Exception;

  class InvalidArgument_ {};
  typedef
  Compound <InvalidArgument_, Exception>
  InvalidArgument;

  class NotInitialized_ {};
  typedef
  Compound <NotInitialized_, Exception>
  NotInitialized;

public:

  void
  foo (char const* str)
  {
    // This is just an example.

    if (str == 0)
    {
      throw InvalidArgument ("Base::foo: first parameter is zero.");
    }
    else
    {
      ostringstream ostr;
      ostr << "Base::foo [this = " << this << "]: object is not initialized.";

      throw NotInitialized (ostr);
    }
  }


  // We don't know what implementation may decide to throw so
  // we allow to throw System exception and any logic exception
  // derived from Base::Exception
  virtual void
  vfoo () = 0;
};

class Derived : public Base
{
public:

  // Define some Derived-specific logic exception.
  class NotImplemented_ {};
  typedef
  Compound <NotImplemented_, Exception>
  NotImplemented;

public:
  virtual void
  vfoo ()
  {
    std::string str ("Derived::vfoo: not implemented yet.");
    throw NotImplemented (str);
  }
};

int
main ()
{
  try
  {

    Derived d;
    Base* pb (&d);

    // We can use generic handler.
    try
    {
      pb->vfoo ();
    }
    catch (Base::Exception const& ex)
    {
      cerr << "Caught Base::Exception: " << ex.what () << endl;
    }


    // Or use more precise control.
    try
    {
      pb->foo ("hello");
    }
    catch (Base::NotInitialized const& ex)
    {
      cerr << "Caught Base::NotInitialized: " << ex.what () << endl;
    }

    // Or use application-level handler.
    pb->foo (0);

  }
  catch (Logic::Exception const& ex)
  {
    cerr << "Caught Logic::Exception: " << ex.what () << endl;
  }
  catch (...)
  {
    cerr << "Caught unknown exception using catch-all handler" << endl;
    return -1;
  }

}
//$Id$
