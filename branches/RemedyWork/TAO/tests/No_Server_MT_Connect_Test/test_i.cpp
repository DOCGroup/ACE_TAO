// $Id$

#include "test_i.h"

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Simple_Server_i::test_method (void)
{
}

void
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
