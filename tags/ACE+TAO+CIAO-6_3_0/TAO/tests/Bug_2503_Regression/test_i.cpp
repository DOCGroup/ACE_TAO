// $Id$

#include "test_i.h"

test_i::
test_i(CORBA::ORB_ptr orb)
{
 this->orb_ = CORBA::ORB::_duplicate (orb);
}

void test_i::
the_operation(CORBA::Long & x)
{
  x = 42;
}

char *
test_i::
create_and_activate_server()
{
  PortableServer::Servant_var<test_i> impl(
      new test_i (this->orb_.in ()));

  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::ObjectId_var id =
    root_poa->activate_object (impl.in ());

  CORBA::Object_var object = root_poa->id_to_reference (id.in ());

  Test_var ref = Test::_narrow (object.in ());

  return this->orb_->object_to_string(ref.in());
}

void
test_i::shutdown (void)
{
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown (0);
}
