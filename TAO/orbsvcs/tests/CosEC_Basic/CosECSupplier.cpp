/* -*- C++ -*- */
// $Id$

#include "CosECSupplier.h"

void
CosECSupplier::open (CosEventChannelAdmin::EventChannel_ptr event_channel,
                     CORBA::Environment& ACE_TRY_ENV)
{
  // = Connect as a consumer.
  this->supplier_admin_ =
    event_channel->for_suppliers (ACE_TRY_ENV);
  ACE_CHECK;
}

void
CosECSupplier::close (CORBA::Environment &ACE_TRY_ENV)
{
  this->disconnect (ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_admin_ =
    CosEventChannelAdmin::SupplierAdmin::_nil ();
}

void
CosECSupplier::connect (CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_ =
    this->supplier_admin_->obtain_push_consumer (ACE_TRY_ENV);
  ACE_CHECK;

  CosEventComm::PushSupplier_var objref = this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_proxy_->connect_push_supplier (objref.in (),
                                                ACE_TRY_ENV);
  ACE_CHECK;
}

void
CosECSupplier::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ())
      || CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_proxy_ =
    CosEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
CosECSupplier::send_event (const CORBA::Any & data,
                           CORBA::Environment &ACE_TRY_ENV)
{
  this->consumer_proxy_->push (data, ACE_TRY_ENV);
  ACE_CHECK;
}

void
CosECSupplier::disconnect_push_supplier (CORBA::Environment &ACE_TRY_ENV)
{
  // Deactivate this object.

  PortableServer::POA_var poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this,
                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}
