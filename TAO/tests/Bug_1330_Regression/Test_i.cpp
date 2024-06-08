#include "Test_i.h"

Test_i::Test_i(CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test_i::~Test_i()
{
}

void
Test_i::test_method ()
{
}

void
Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}
