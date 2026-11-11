// Implements a PushConsumer.

#include "Consumer_i.h"
#include <iostream>

#include "tao/PortableServer/PS_CurrentC.h"

Consumer_i::Consumer_i (CORBA::ORB_ptr orb,
                        CORBA::Object_ptr obj)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    object_ (CORBA::Object::_duplicate (obj))
{
}

CORBA::Object_ptr
Consumer_i::get_typed_consumer ()
{
  return CORBA::Object::_duplicate (this->object_.in ());
}

// Override the push() operation.
void Consumer_i::push (CORBA::Any const &)
{
  throw CORBA::NO_IMPLEMENT ();
}

// Override the disconnect_push_consumer() operation.
void Consumer_i::disconnect_push_consumer ()
{
  // Deactivate this object.
  CORBA::Object_var const obj = this->orb_->resolve_initial_references ("POACurrent");
  PortableServer::Current_var const current = PortableServer::Current::_narrow (obj.in ());
  PortableServer::POA_var const poa = current->get_POA ();
  PortableServer::ObjectId_var const object_id = current->get_object_id ();
  poa->deactivate_object (object_id.in ());
}
