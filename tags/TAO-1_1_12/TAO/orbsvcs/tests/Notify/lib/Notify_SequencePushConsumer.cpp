// $Id$

#include "Notify_SequencePushConsumer.h"

ACE_RCSID(Notify_Tests, Notify_SequencePushConsumer, "$Id$")

TAO_Notify_SequencePushConsumer::TAO_Notify_SequencePushConsumer (void)
{
  // No-Op.
}

TAO_Notify_SequencePushConsumer::~TAO_Notify_SequencePushConsumer (void)
{
  // release all resources ...
  this->default_POA_ = PortableServer::POA::_nil ();

  this->proxy_supplier_ =
    CosNotifyChannelAdmin::SequenceProxyPushSupplier::_nil ();
}

void TAO_Notify_SequencePushConsumer::init (PortableServer::POA_ptr poa, CORBA::Environment &/*ACE_TRY_ENV*/)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

PortableServer::POA_ptr
TAO_Notify_SequencePushConsumer::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}


CosNotifyChannelAdmin::SequenceProxyPushSupplier_ptr
TAO_Notify_SequencePushConsumer::get_proxy_supplier (void)
{
  return proxy_supplier_.in ();
}

void
TAO_Notify_SequencePushConsumer::connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin, CORBA::Environment &ACE_TRY_ENV)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::SequencePushConsumer_var consumer_ref =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (CosNotifyChannelAdmin::SEQUENCE_EVENT, proxy_supplier_id_, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ = CosNotifyChannelAdmin::SequenceProxyPushSupplier::
    _narrow (proxysupplier.in (), ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  proxy_supplier_->connect_sequence_push_consumer (consumer_ref.in (),
                                                     ACE_TRY_ENV);
  // give ownership to POA
  this->_remove_ref ();
}

void
TAO_Notify_SequencePushConsumer::deactivate (CORBA::Environment &ACE_TRY_ENV)
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
TAO_Notify_SequencePushConsumer::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  this->proxy_supplier_->disconnect_sequence_push_supplier(ACE_TRY_ENV);
  ACE_CHECK;

  this->deactivate (ACE_TRY_ENV);
}

void
TAO_Notify_SequencePushConsumer::offer_change (const CosNotification::EventTypeSeq & /*added*/, const CosNotification::EventTypeSeq & /*removed*/, CORBA::Environment & //ACE_TRY_ENV
)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ))
{
  // No-Op.
}

void
TAO_Notify_SequencePushConsumer::disconnect_sequence_push_consumer (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->deactivate (ACE_TRY_ENV);
}

void
TAO_Notify_SequencePushConsumer:: push_structured_events (const CosNotification::EventBatch &/*notifications*/, CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
}
