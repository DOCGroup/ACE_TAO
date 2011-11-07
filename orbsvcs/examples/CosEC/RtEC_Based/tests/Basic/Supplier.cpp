/* -*- C++ -*- */
// $Id$

#include "Supplier.h"

void
Supplier::open (CosEventChannelAdmin::EventChannel_ptr event_channel)
{
  // = Connect as a consumer.
  this->supplier_admin_ =
    event_channel->for_suppliers ();
}

void
Supplier::close (void)
{
  this->disconnect ();

  this->supplier_admin_ =
    CosEventChannelAdmin::SupplierAdmin::_nil ();
}

void
Supplier::connect (void)
{
  if (CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_ =
    this->supplier_admin_->obtain_push_consumer ();

  CosEventComm::PushSupplier_var objref = this->_this ();

  this->consumer_proxy_->connect_push_supplier (objref.in ());
}

void
Supplier::disconnect (void)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ())
      || CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer ();

  this->consumer_proxy_ =
    CosEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
Supplier::send_event (const CORBA::Any & data)
{
  this->consumer_proxy_->push (data);
}

void
Supplier::disconnect_push_supplier (void)
{
  // Deactivate this object.

  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}
