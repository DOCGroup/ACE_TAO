/**
 * @file
 *
 * This program checks if the compiler / platform supports the
 * std::unique_ptr<> correctly.  The motivation for this test was a discussion
 * on the development mailing list, and the documentation was captured
 * in:
 *
 *   https://github.com/DOCGroup/bugzilla/issues/3715
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
    ++constructors;
  }
  Base(Base const & )
  {
    ++constructors;
  }
  ~Base()
  {
    ++destructors;
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

  std::unique_ptr<Base> x = std::make_unique<Base> ();
  std::unique_ptr<Derived> y = std::make_unique<Derived> ();

  x = std::move (y);

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
  if (x)
    {
      status = 1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("x contents should have been transferred\n")
                 ));
    }

  ACE_END_TEST;
  return status;
}

