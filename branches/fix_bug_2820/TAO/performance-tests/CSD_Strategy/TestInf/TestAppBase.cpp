// $Id$
#include "TestAppBase.h"

TestAppBase::TestAppBase(const char* name)
  : name_(name)
{
}


TestAppBase::~TestAppBase()
{
}


const char*
TestAppBase::name() const
{
  return this->name_.c_str();
}


int
TestAppBase::run(int argc, char* argv[])
{
  int rc = this->run_i(argc, argv);

  // Convert 1 to 0.  Leave 0 and -1 as they are.
  return (rc == 1) ? 0 : rc;
}
