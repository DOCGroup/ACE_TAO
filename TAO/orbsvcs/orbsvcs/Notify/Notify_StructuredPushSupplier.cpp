/* -*- C++ -*- $Id$ */
#include "Notify_StructuredPushSupplier.h"

TAO_Notify_StructuredPushSupplier::TAO_Notify_StructuredPushSupplier (void)
{
}

TAO_Notify_StructuredPushSupplier::~TAO_Notify_StructuredPushSupplier ()
{
}

CosNotifyComm::StructuredPushSupplier_ptr
TAO_Notify_StructuredPushSupplier::get_ref (void)
{
  return _this ();
}

void
TAO_Notify_StructuredPushSupplier::open
(CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin,
 CORBA::Environment& /*ACE_TRY_ENV*/)
{
  supplier_admin_ =
    CosNotifyChannelAdmin::SupplierAdmin::_duplicate (supplier_admin);
}

void
TAO_Notify_StructuredPushSupplier:: close (CORBA::Environment &ACE_TRY_ENV)
{
  this->disconnect (ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_admin_ =
    CosNotifyChannelAdmin::SupplierAdmin::_nil ();
}

void
TAO_Notify_StructuredPushSupplier::connect (CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  CosNotifyComm::StructuredPushSupplier_var objref =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin_->obtain_notification_push_consumer
    (CosNotifyChannelAdmin::STRUCTURED_EVENT,
     supplieradmin_proxy_id_,
     ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->consumer_proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::
    _narrow (proxyconsumer.in (),
             ACE_TRY_ENV);
  ACE_CHECK;
  ACE_ASSERT (!CORBA::is_nil (consumer_proxy_.in ()));

  consumer_proxy_->connect_structured_push_supplier (objref.in (),
                                                     ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_StructuredPushSupplier::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
      return;

  this->consumer_proxy_->disconnect_structured_push_consumer(ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_nil ();
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
}

void
TAO_Notify_StructuredPushSupplier::disconnect_structured_push_supplier (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
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

void
TAO_Notify_StructuredPushSupplier::send_event
(const CosNotification::StructuredEvent& event,
 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_ASSERT (!CORBA::is_nil (this->consumer_proxy_.in ()));

  consumer_proxy_->push_structured_event (event, ACE_TRY_ENV);
  ACE_CHECK;
}
