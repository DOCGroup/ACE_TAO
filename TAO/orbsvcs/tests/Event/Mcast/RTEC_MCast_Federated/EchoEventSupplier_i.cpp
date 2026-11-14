// EchoEventSupplier_i.cpp
// Implements a PushSupplier.

#include "EchoEventSupplier_i.h"
#include "tao/PortableServer/PS_CurrentC.h"

EchoEventSupplier_i::EchoEventSupplier_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

// Override the disconnect_push_Supplier() operation.
void EchoEventSupplier_i::disconnect_push_supplier ()
{
  // Deactivate this object.
  CORBA::Object_var obj = this->orb_->resolve_initial_references ("POACurrent");
  PortableServer::Current_var current = PortableServer::Current::_narrow (obj.in ());
  PortableServer::POA_var poa = current->get_POA ();
  PortableServer::ObjectId_var object_id = current->get_object_id ();
  poa->deactivate_object (object_id.in ());
}
