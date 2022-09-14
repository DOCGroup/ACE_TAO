#include "ORT_test_i.h"

ORT_test_i::ORT_test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Boolean
ORT_test_i::request_server ()
{
  return 0;
}

void
ORT_test_i::shutdown ()
{
  this->orb_->shutdown (false);
}
