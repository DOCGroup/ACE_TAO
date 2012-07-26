// $Id$

// Implements a PushConsumer.

#include "Consumer_i.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include <iostream>

Consumer_i::Consumer_i(CORBA::ORB_ptr orb,
                       CORBA::Object_ptr obj)
  : orb_(CORBA::ORB::_duplicate(orb))
  , object_(CORBA::Object::_duplicate(obj))
{
}


CORBA::Object_ptr
Consumer_i::get_typed_consumer ()
{
  return CORBA::Object::_duplicate(object_.in());
}

// Override the push() operation.
void Consumer_i::push(const CORBA::Any &)
{
  throw CORBA::NO_IMPLEMENT ();
}

// Override the disconnect_push_consumer() operation.
void Consumer_i::disconnect_push_consumer()
{
  // Deactivate this object.
  CORBA::Object_var obj = orb_->resolve_initial_references("POACurrent");
  PortableServer::Current_var current = PortableServer::Current::_narrow(obj.in());
  PortableServer::POA_var poa = current->get_POA();
  PortableServer::ObjectId_var objectId = current->get_object_id();
  poa->deactivate_object(objectId.in());
}
