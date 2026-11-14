/* -*- C++ -*- */
#include "Consumer.h"

void
Consumer::open (CosEventChannelAdmin::EventChannel_ptr const event_channel)
{
  this->consumer_admin_ =
    event_channel->for_consumers ();
}

void
Consumer::close ()
{
  this->disconnect ();

  this->consumer_admin_ =
    CosEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
Consumer::connect ()
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  CosEventComm::PushConsumer_var const objref =
    this->_this ();

  this->supplier_proxy_ =
    this->consumer_admin_->obtain_push_supplier ();

  this->supplier_proxy_->connect_push_consumer (objref.in ());
}

void
Consumer::disconnect ()
{
  if (CORBA::is_nil (this->supplier_proxy_.in ())
      || CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier ();

  this->supplier_proxy_ =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
Consumer::push (CORBA::Any const &)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("in Consumer::push\n")));
  // @@ read comment in run
}

void
Consumer::disconnect_push_consumer ()
{
  // Deactivate this object.

  PortableServer::POA_var const poa =
    this->_default_POA ();

  PortableServer::ObjectId_var const id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}
