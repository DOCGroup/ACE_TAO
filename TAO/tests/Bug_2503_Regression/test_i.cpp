#include "test_i.h"

test_i::
test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void test_i::
the_operation (CORBA::Long &x)
{
  x = 42;
}

CORBA::String_var
test_i::create_and_activate_server () const
{
  // Servant_var owns the newly allocated servant reference.
  PortableServer::Servant_var<test_i> const impl (
    new test_i (this->orb_.in ()));

  CORBA::Object_var const poa_object =
    this->orb_->resolve_initial_references ("RootPOA");

  PortableServer::POA_var const root_poa =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::ObjectId_var const id =
    root_poa->activate_object (impl.in ());

  CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

  Test_var const ref = Test::_narrow (object.in ());

  return CORBA::String_var (this->orb_->object_to_string (ref.in ()));
}

void
test_i::shutdown ()
{
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown (false);
}
