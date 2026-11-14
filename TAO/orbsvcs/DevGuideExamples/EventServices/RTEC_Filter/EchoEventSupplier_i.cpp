// EchoEventSupplier_i.cpp
// Implements a PushSupplier.

#include "tao/PortableServer/PS_CurrentC.h"
#include "EchoEventSupplier_i.h"

EchoEventSupplier_i::EchoEventSupplier_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

// Override the disconnect_push_Supplier() operation.
void EchoEventSupplier_i::disconnect_push_supplier ()
{
  // Deactivate this object.
  CORBA::Object_var const obj = this->orb_->resolve_initial_references ("POACurrent");
  PortableServer::Current_var const current = PortableServer::Current::_narrow (obj.in ());
  PortableServer::POA_var const poa = current->get_POA ();
  PortableServer::ObjectId_var const object_id = current->get_object_id ();
  poa->deactivate_object (object_id.in ());
}
