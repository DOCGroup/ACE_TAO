// -*- C++ -*-
// $Id$
#include "Notify_StructuredPushSupplier.h"

// @@ Pradeep: the RCSID

TAO_Notify_StructuredPushSupplier::TAO_Notify_StructuredPushSupplier (void)
{
}

TAO_Notify_StructuredPushSupplier::~TAO_Notify_StructuredPushSupplier ()
{
}

void
TAO_Notify_StructuredPushSupplier::init (PortableServer::POA_ptr poa, CORBA::Environment & /*ACE_TRY_ENV*/)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

void
TAO_Notify_StructuredPushSupplier::connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin, CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyComm::StructuredPushSupplier_var objref =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (CosNotifyChannelAdmin::STRUCTURED_EVENT, my_id_, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->consumer_proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxyconsumer.in (), ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (consumer_proxy_.in ()));

  consumer_proxy_->connect_structured_push_supplier (objref.in (),
                                                     ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_StructuredPushSupplier::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_ASSERT (!CORBA::is_nil (this->consumer_proxy_.in ()));

  this->consumer_proxy_->disconnect_structured_push_consumer(ACE_TRY_ENV);
  ACE_CHECK;

  this->deactivate (ACE_TRY_ENV);
}

void
TAO_Notify_StructuredPushSupplier::subscription_change (
                                                        const CosNotification::EventTypeSeq & /*added*/,
                                                        const CosNotification::EventTypeSeq & /*removed */,
                                                        CORBA::Environment & //ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ))
{
  //No-Op.
}

PortableServer::POA_ptr
TAO_Notify_StructuredPushSupplier::_default_POA (CORBA::Environment& /* env */)
{
  return PortableServer::POA::_duplicate (this->default_POA_);
}

void
TAO_Notify_StructuredPushSupplier::deactivate (CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::ObjectId_var id =
    this->default_POA_->servant_to_id (this,
                                       ACE_TRY_ENV);
  ACE_CHECK;

  this->default_POA_->deactivate_object (id.in (),
                                         ACE_TRY_ENV);
  ACE_CHECK;

  // release all resources ...
  this->default_POA_ = PortableServer::POA::_nil ();

  this->consumer_proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_nil ();
}

void
TAO_Notify_StructuredPushSupplier::disconnect_structured_push_supplier (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  this->consumer_proxy_->disconnect_structured_push_consumer (ACE_TRY_ENV);
  ACE_CHECK;
  this->deactivate (ACE_TRY_ENV);
}

void
TAO_Notify_StructuredPushSupplier::send_event (const CosNotification::StructuredEvent& event, CORBA::Environment &ACE_TRY_ENV)
{
  ACE_ASSERT (!CORBA::is_nil (this->consumer_proxy_.in ()));

  consumer_proxy_->push_structured_event (event, ACE_TRY_ENV);
  ACE_CHECK;
}
