/* -*- C++ -*- */
// $Id$

#include "Supplier.h"

void
Supplier::open (CosEventChannelAdmin::EventChannel_ptr event_channel
                     TAO_ENV_ARG_DECL)
{
  // = Connect as a consumer.
  this->supplier_admin_ =
    event_channel->for_suppliers (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Supplier::close (TAO_ENV_SINGLE_ARG_DECL)
{
  this->disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_admin_ =
    CosEventChannelAdmin::SupplierAdmin::_nil ();
}

void
Supplier::connect (TAO_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_ =
    this->supplier_admin_->obtain_push_consumer (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosEventComm::PushSupplier_var objref = this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_proxy_->connect_push_supplier (objref.in ()
                                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Supplier::disconnect (TAO_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ())
      || CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_proxy_ =
    CosEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
Supplier::send_event (const CORBA::Any & data
                           TAO_ENV_ARG_DECL)
{
  this->consumer_proxy_->push (data TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Supplier::disconnect_push_supplier (TAO_ENV_SINGLE_ARG_DECL)
 ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  // Deactivate this object.

  PortableServer::POA_var poa =
    this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
