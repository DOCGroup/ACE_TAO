#include "bug_689_regression_i.h"

bug_689_regression_i::bug_689_regression_i (CORBA::ORB_ptr orb)
 : orb_ (CORBA::ORB::_duplicate (orb))
{
}

bug_689_regression_i::~bug_689_regression_i ()
{
}

void
bug_689_regression_i::shutdown ()
{
  this->orb_->shutdown (false);
}

void bug_689_regression_i::noop ()
{
}
