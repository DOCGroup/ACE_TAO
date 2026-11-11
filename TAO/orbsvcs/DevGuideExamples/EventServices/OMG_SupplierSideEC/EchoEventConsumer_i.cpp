// Implements a PushConsumer.

#include "EchoEventConsumer_i.h"
#include <iostream>

#include "tao/PortableServer/PS_CurrentC.h"

EchoEventConsumer_i::EchoEventConsumer_i (
  CORBA::ORB_ptr orb,
  CosEventChannelAdmin::ProxyPushSupplier_ptr supplier,
  int event_limit)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    supplier_ (CosEventChannelAdmin::ProxyPushSupplier::_duplicate (supplier)),
    event_limit_ (event_limit)
{
}

// Override the push() operation.
void
EchoEventConsumer_i::push (CORBA::Any const &data)
{
  char const *event_data {};
  if (data >>= event_data)
    {
      std::cout << "EchoEventConsumer_i::push(): Received event: "
                << event_data << std::endl;
    }

  if (--this->event_limit_ <= 0)
    {
      this->supplier_->disconnect_push_supplier ();
      this->orb_->shutdown (0);
    }
}

// Override the disconnect_push_consumer() operation.
void
EchoEventConsumer_i::disconnect_push_consumer ()
{
  CORBA::Object_var const obj = this->orb_->resolve_initial_references ("POACurrent");
  PortableServer::Current_var const current = PortableServer::Current::_narrow (obj.in ());
  PortableServer::POA_var const poa = current->get_POA ();
  PortableServer::ObjectId_var const object_id = current->get_object_id ();
  poa->deactivate_object (object_id.in ());
}
