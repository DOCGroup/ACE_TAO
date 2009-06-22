// $Id$

// Implements a PushConsumer.

#include "EchoEventConsumer_i.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include <iostream>

EchoEventConsumer_i::EchoEventConsumer_i(
                      CORBA::ORB_ptr orb,
                      CosEventChannelAdmin::ProxyPushSupplier_ptr supplier,
                      int event_limit)
  : orb_(CORBA::ORB::_duplicate(orb))
  , supplier_(CosEventChannelAdmin::ProxyPushSupplier::_duplicate(supplier))
  , event_limit_(event_limit)
{
}

// Override the push() operation.
void EchoEventConsumer_i::push(const CORBA::Any & data)
{
  // Extract event data from the any.
  const char* eventData;
  if (data >>= eventData)
  {
    std::cout << "EchoEventConsumer_i::push(): Received event: "
         << eventData << std::endl;
  }
  if (--event_limit_ <= 0) {
    supplier_->disconnect_push_supplier();
    orb_->shutdown(0);
  }
}

// Override the disconnect_push_consumer() operation.
void EchoEventConsumer_i::disconnect_push_consumer()
{
  // Deactivate this object.
  CORBA::Object_var obj = orb_->resolve_initial_references("POACurrent");
  PortableServer::Current_var current = PortableServer::Current::_narrow(obj.in());
  PortableServer::POA_var poa = current->get_POA();
  PortableServer::ObjectId_var objectId = current->get_object_id();
  poa->deactivate_object(objectId.in());
}
