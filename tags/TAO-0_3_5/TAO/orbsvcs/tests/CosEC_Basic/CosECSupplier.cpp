/* -*- C++ -*- */
// $Id$

#include "CosECSupplier.h"

void
CosECSupplier::open (CosEventChannelAdmin::EventChannel_ptr event_channel,
                     CORBA::Environment& TAO_TRY_ENV)
{
  // = Connect as a consumer.
  this->supplier_admin_ =
    event_channel->for_suppliers (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);
}

void
CosECSupplier::close (CORBA::Environment &TAO_TRY_ENV)
{
  this->disconnect (TAO_TRY_ENV);
  this->supplier_admin_ =
    CosEventChannelAdmin::SupplierAdmin::_nil ();
}

void
CosECSupplier::connect (CORBA::Environment &TAO_TRY_ENV)
{
  if (CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_ =
    this->supplier_admin_->obtain_push_consumer (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  CosEventComm::PushSupplier_var objref = this->_this (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  this->consumer_proxy_->connect_push_supplier (objref.in (),
                                                TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);
}

void
CosECSupplier::disconnect (CORBA::Environment &TAO_TRY_ENV)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ())
      || CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  this->consumer_proxy_ =
    CosEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
CosECSupplier::send_event (const CORBA::Any & data,
                           CORBA::Environment &TAO_TRY_ENV)
{
  this->consumer_proxy_->push (data, TAO_TRY_ENV);
}

void
CosECSupplier::disconnect_push_supplier (CORBA::Environment &TAO_TRY_ENV)
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
