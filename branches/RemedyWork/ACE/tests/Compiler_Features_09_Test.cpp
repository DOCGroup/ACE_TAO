// $Id$

/**
 * @file
 *
 * This program checks if the compiler / platform supports the
 * std::auto_ptr<> correctly.  The motivation for this test was a discussion
 * on the development mailing list, and the documentation was captured
 * in:
 *
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=3715
 *
 */

#include "test_config.h"

// The first part of the test is to compile this line.  If the program
// does not compile the platform is just too broken.
#include <memory>

// For extra challenge, we use the anonymous namespace
namespace
{
/**
 * @class Base
 */
class Base
{
public:
  Base()
  {
    constructors++;
  }
  Base(Base const & )
  {
    constructors++;
  }
  ~Base()
  {
    destructors++;
  }

  static int constructors;
  static int destructors;
};

int Base::constructors = 0;
int Base::destructors = 0;

class Derived : public Base
{
public:
  Derived()
    : Base()
  {}
};
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_09_Test"));

  // As usual, the exit status from the test is 0 on success, 1 on
  // failure
  int status = 0;

  // ... this works with the ACE version of auto_ptr (well, the
  // namespace is broken, but you get the idea) ...
  std::auto_ptr<Base> x(new Base);
  std::auto_ptr<Derived> y(new Derived);

  // ... with a compliant implementation of std::auto_ptr<> you should be
  // able to write:
  //    x = y;
  // but the Solaris compiler was broken as of August, 2009!!  So you have
  // to work around in the following way.  This compiler is important
  // enough for the ACE community, so we have to support this broken
  // configuration ...
  x.reset(y.release());

  // ... there should be just one destruction so far ...
  if (Base::destructors != 1)
    {
      status = 1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("Destructor count off, expected 1, found %d\n"),
                 Base::destructors));
    }

  std::auto_ptr<Base> z;
  z = x;
  if (Base::destructors != 1)
    {
      status = 1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("Destructor count off, expected 1, found %d\n"),
                 Base::destructors));
    }
  if (x.get())
    {
      status = 1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("x contents should have been transferred\n")
                 ));
    }

  ACE_END_TEST;
  return status;
}


