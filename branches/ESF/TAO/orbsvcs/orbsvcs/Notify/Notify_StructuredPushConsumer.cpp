/* -*- C++ -*- $Id$ */
#include "Notify_StructuredPushConsumer.h"

TAO_Notify_StructuredPushConsumer::TAO_Notify_StructuredPushConsumer (void)
{
}

TAO_Notify_StructuredPushConsumer::~TAO_Notify_StructuredPushConsumer (void)
{
}

CosNotifyComm::StructuredPushConsumer_ptr
TAO_Notify_StructuredPushConsumer::get_ref (void)
{
  return _this ();
}

void
TAO_Notify_StructuredPushConsumer::open (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin, CORBA::Environment& /* ACE_TRY_ENV */)
{
  consumer_admin_ =
    CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (consumer_admin);
}

void
TAO_Notify_StructuredPushConsumer::close (CORBA::Environment &ACE_TRY_ENV)
{
  this->disconnect (ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_admin_ =
    CosNotifyChannelAdmin::ConsumerAdmin::_nil ();
}

void
TAO_Notify_StructuredPushConsumer::connect (CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin_->obtain_notification_push_supplier
    (CosNotifyChannelAdmin::STRUCTURED_EVENT,
     consumeradmin_proxy_id_,
     ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->supplier_proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::
    _narrow (proxysupplier.in (),
             ACE_TRY_ENV);
  ACE_CHECK;
  ACE_ASSERT (!CORBA::is_nil (supplier_proxy_.in ()));

  supplier_proxy_->connect_structured_push_consumer (objref.in (),
                                                     ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_StructuredPushConsumer::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
      return;

  this->supplier_proxy_->disconnect_structured_push_supplier(ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_nil ();
}

void
TAO_Notify_StructuredPushConsumer::offer_change (
                                                 const CosNotification::EventTypeSeq & /*added*/,
                                                 const CosNotification::EventTypeSeq & /*removed*/,
                                                 CORBA::Environment & //ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ))
{
}

/*
void
TAO_Notify_StructuredPushConsumer::push_structured_event (
        const CosNotification::StructuredEvent & notification,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
      ))
{
  ACE_DEBUG ((LM_DEBUG,
              "in TAO_Notify_StructuredPushConsumer::push\n"));
}
*/

void
TAO_Notify_StructuredPushConsumer::disconnect_structured_push_consumer (
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
