/* -*- C++ -*- */
// $Id$

#include "CosECConsumer.h"

void
CosECConsumer::open (CosEventChannelAdmin::EventChannel_ptr event_channel,
                     CORBA::Environment& TAO_TRY_ENV)
{
  // = Connect as a consumer.
  this->consumer_admin_ =
    event_channel->for_consumers (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);
}

void
CosECConsumer::close (CORBA::Environment &TAO_TRY_ENV)
{
  this->disconnect (TAO_TRY_ENV);
  this->consumer_admin_ =
    CosEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
CosECConsumer::connect (CORBA::Environment &TAO_TRY_ENV)
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  CosEventComm::PushConsumer_var objref =
    this->_this (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  this->supplier_proxy_ =
    this->consumer_admin_->obtain_push_supplier (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  this->supplier_proxy_->connect_push_consumer (objref.in (),
                                                TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);
}

void
CosECConsumer::disconnect (CORBA::Environment &TAO_TRY_ENV)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ())
      || CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  this->supplier_proxy_ =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
CosECConsumer::push (const CORBA::Any &data,
                     CORBA::Environment &TAO_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "in CosECConsumer::push\n"));
}

void
CosECConsumer::disconnect_push_consumer (CORBA::Environment &TAO_TRY_ENV)
{
  // Deactivate this object.

  PortableServer::POA_var poa =
    this->_default_POA (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this,
                        TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  poa->deactivate_object (id.in (),
                          TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);
}
