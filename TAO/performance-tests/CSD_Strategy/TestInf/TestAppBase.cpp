#include "TestAppBase.h"

TestAppBase::TestAppBase (char const *name)
  : name_ (name)
{
}

TestAppBase::~TestAppBase () = default;

char const *
TestAppBase::name () const
{
  return this->name_.c_str ();
}

int
TestAppBase::run (int argc, ACE_TCHAR *argv[])
{
  int const rc = this->run_i (argc, argv);

  // Convert 1 to 0.  Leave 0 and -1 as they are.
  return (rc == 1) ? 0 : rc;
}
