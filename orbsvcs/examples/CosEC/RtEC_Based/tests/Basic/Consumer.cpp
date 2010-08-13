/* -*- C++ -*- */
// $Id$

#include "Consumer.h"

void
Consumer::open (CosEventChannelAdmin::EventChannel_ptr event_channel,
                     CORBA::ORB_ptr orb)
{
  this->orb_ = orb;

  // = Connect as a consumer.
  this->consumer_admin_ =
    event_channel->for_consumers ();
}

void
Consumer::close (void)
{
  this->disconnect ();

  this->consumer_admin_ =
    CosEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
Consumer::connect (void)
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  CosEventComm::PushConsumer_var objref =
    this->_this ();

  this->supplier_proxy_ =
    this->consumer_admin_->obtain_push_supplier ();

  this->supplier_proxy_->connect_push_consumer (objref.in ());
}

void
Consumer::disconnect (void)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ())
      || CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier ();

  this->supplier_proxy_ =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
Consumer::push (const CORBA::Any &)
{
  ACE_DEBUG ((LM_DEBUG,
              "in Consumer::push\n"));
  // this->orb_->shutdown ();
  // @@ read comment in run
}

void
Consumer::disconnect_push_consumer (void)
{
  // Deactivate this object.

  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}
