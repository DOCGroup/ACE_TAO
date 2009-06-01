// file      : Example/ExH/HelloWorld/hello_world.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

/* FUZZ: disable check_for_improper_main_declaration */

#include <cstdlib> // for std::abort ()

#include <string>
#include <iostream>

#include "Utility/ExH/System/Exception.hpp"
#include "Utility/ExH/Logic/Exception.hpp"

using std::cerr;
using std::cout;
using std::endl;

using namespace Utility;

class Application
{
public:
  class Exception : public ExH::Logic::Exception {};

  // Hint: you may want to try again...
  class FeelingDizzy  : public Exception {};

  class InvalidArg : public Exception {};

public:
  Application ()
      : // The std::string c-tor may throw any kind of exceptions besides
        // quite possible std::bad_alloc.
        greeting_ ("Hello, world!")
  {
  }

  Application (char const * greeting)
      : greeting_ (greeting == 0 ? "" : greeting)
  {
    if (greeting == 0) throw InvalidArg ();
  }

public:

  void
  run ()
  {
    static unsigned int dizzy_count (0);

    if (dizzy_count++ < 5) throw FeelingDizzy ();

    // The next line can throw full bucket of exceptions
    // not to mention ios_base::failure.
    cout << greeting_.c_str () << endl;
  }

private:

  std::string  greeting_;
};



int
main ()
{
  // This is a catch-all layer that should be in use only
  // if we are really in trouble.
  try
  {
    // This is a catch-system layer. Here we will catch exceptions like
    // bad_alloc, etc. If we get here it means that nobody wanted/managed
    // to recover from this kind of errors.
    try
    {
      // This is a catch-logic layer. If we get here it usually
      // indicates an application logic error.
      try
      {

        // Ok, here we go about our application logic.
        try
        {
          for (int i = 0; i < 10; i++)
          {
            try
            {
              Application app ("Hi dude!");
              app.run ();
              break;
            }
            catch (Application::FeelingDizzy const& )
            {
              if (i == 9)
              {
                cerr << "Given up!" << endl;
                return -1;
              }
              else
              {
                cerr << "Application is feeling dizzy. Trying again..."
                     << endl;
              }
            }
          }
        }
        catch (Application::InvalidArg const& )
        {
          cerr << "Cought Application::InvalidArg : ...hmm... strange!"
               << endl;
          return -1;
        }
      }
      catch (ExH::Logic::Exception const& e)
      {
        cerr << "Caught Logic::Exception : " << e.what () << endl;
        return -1;
      }
    }
    catch (const ExH::System::Exception& e)
    {
      cerr << "Caught System::Exception : " << e.what () << endl;
      return -1;
    }
    catch (...)
    {
      cerr << "Caught unknown exception using catch-all handler. " << endl;
      return -1;
    }
  }
  catch (...)
  {
    // We get here in cases of some hard failure. For example when handling
    // exception, operator << throws another exception. Usually application
    // cannot handle such failures itself so we just propagate it futher.
    std::abort ();
  }
}
//$Id$
