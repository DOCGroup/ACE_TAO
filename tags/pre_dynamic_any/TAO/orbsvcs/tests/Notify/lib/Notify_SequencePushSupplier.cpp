// -*- C++ -*-
// $Id$

#include "Notify_SequencePushSupplier.h"

ACE_RCSID(Notify_Tests, Notify_SequencePushSupplier, "$Id$")

TAO_Notify_SequencePushSupplier::TAO_Notify_SequencePushSupplier (void)
{
}

TAO_Notify_SequencePushSupplier::~TAO_Notify_SequencePushSupplier ()
{
  // release all resources ...
  this->default_POA_ = PortableServer::POA::_nil ();

  this->proxy_consumer_ =
    CosNotifyChannelAdmin::SequenceProxyPushConsumer::_nil ();
}

void
TAO_Notify_SequencePushSupplier::init (PortableServer::POA_ptr poa, CORBA::Environment & /*ACE_TRY_ENV*/)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

CosNotifyChannelAdmin::SequenceProxyPushConsumer_ptr
TAO_Notify_SequencePushSupplier::get_proxy_consumer (void)
{
  return proxy_consumer_.in ();
}

void
TAO_Notify_SequencePushSupplier::connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin, CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyComm::SequencePushSupplier_var supplier_ref =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (CosNotifyChannelAdmin::SEQUENCE_EVENT, proxy_consumer_id_, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::SequenceProxyPushConsumer::_narrow (proxyconsumer.in (), ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer_.in ()));

  proxy_consumer_->connect_sequence_push_supplier (supplier_ref.in (),
                                                   ACE_TRY_ENV);
  ACE_CHECK;

  // give ownership to POA
  this->_remove_ref ();
}

void
TAO_Notify_SequencePushSupplier::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_sequence_push_consumer(ACE_TRY_ENV);
  ACE_CHECK;

  this->deactivate (ACE_TRY_ENV);
}

void
TAO_Notify_SequencePushSupplier::subscription_change (
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
TAO_Notify_SequencePushSupplier::_default_POA (CORBA::Environment& /* env */)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_Notify_SequencePushSupplier::deactivate (CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POA_var poa =
        this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this,
                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_SequencePushSupplier::disconnect_sequence_push_supplier (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  this->proxy_consumer_->disconnect_sequence_push_consumer (ACE_TRY_ENV);
  ACE_CHECK;
  this->deactivate (ACE_TRY_ENV);
}

void
TAO_Notify_SequencePushSupplier::send_events (const CosNotification::EventBatch & notifications, CORBA::Environment &ACE_TRY_ENV)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->push_structured_events (notifications, ACE_TRY_ENV);
  ACE_CHECK;
}
