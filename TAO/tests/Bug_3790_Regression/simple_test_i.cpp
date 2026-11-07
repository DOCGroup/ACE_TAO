
#include "simple_test_i.h"

Simple_Test_i::Simple_Test_i (CORBA::ORB_ptr const orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Simple_Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}
