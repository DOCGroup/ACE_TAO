/**
 * @file
 *
 * This program checks if the compiler / platform supports the
 * std::unique_ptr<> correctly.  The motivation for this test was a discussion
 * on the development mailing list, and the documentation was captured
 * in:
 *
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=3715
 */

#include "test_config.h"

// The first part of the test is to compile this line.  If the program
// does not compile the platform is just too broken.
#include <memory>
#include <utility>

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

  // ... this works with the ACE version of unique_ptr (well, the
  // namespace is broken, but you get the idea) ...
  std::unique_ptr<Base> x(new Base);
  std::unique_ptr<Derived> y(new Derived);

  // ... with a compliant implementation of std::unique_ptr<> you should be
  // able to write:
  //    x = y;
  x.reset(y.release());

  // ... there should be just one destruction so far ...
  if (Base::destructors != 1)
    {
      status = 1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("Destructor count off, expected 1, found %d\n"),
                 Base::destructors));
    }

  std::unique_ptr<Base> z;
  z = std::move(x);
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


