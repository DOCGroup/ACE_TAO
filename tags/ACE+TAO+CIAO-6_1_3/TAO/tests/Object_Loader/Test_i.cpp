// $Id$

#include "Test_i.h"

CORBA::Long
Test_i::instance_count_ = 0;

Test_i::Test_i (PortableServer::POA_ptr poa)
  :  poa_ (PortableServer::POA::_duplicate (poa))
{
  Test_i::instance_count_++;
}

CORBA::Long
Test_i::instance_count (void)
{
  return Test_i::instance_count_;
}

void
Test_i::destroy (void)
{
  PortableServer::POA_var poa =
    this->_default_POA ();
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this);
  poa->deactivate_object (oid.in ());
}

PortableServer::POA_ptr
Test_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
