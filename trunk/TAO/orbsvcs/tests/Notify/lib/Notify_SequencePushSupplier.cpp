// -*- C++ -*-
// $Id$

#include "Notify_SequencePushSupplier.h"

ACE_RCSID(Notify_Tests, Notify_SequencePushSupplier, "$Id$")

TAO_Notify_SequencePushSupplier::TAO_Notify_SequencePushSupplier (void)
{
}

TAO_Notify_SequencePushSupplier::~TAO_Notify_SequencePushSupplier ()
{
  // Release all resources.
  this->default_POA_ = PortableServer::POA::_nil ();

  this->proxy_consumer_ =
    CosNotifyChannelAdmin::SequenceProxyPushConsumer::_nil ();
}

void
TAO_Notify_SequencePushSupplier::init
   (PortableServer::POA_ptr poa  TAO_ENV_ARG_DECL_NOT_USED)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

CosNotifyChannelAdmin::SequenceProxyPushConsumer_ptr
TAO_Notify_SequencePushSupplier::get_proxy_consumer (void)
{
  return proxy_consumer_.in ();
}

void
TAO_Notify_SequencePushSupplier::connect (
    CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin 
    TAO_ENV_ARG_DECL
  )
{
  CosNotifyComm::SequencePushSupplier_var supplier_ref =
    this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (
                        CosNotifyChannelAdmin::SEQUENCE_EVENT, 
                        proxy_consumer_id_ 
                        TAO_ENV_ARG_PARAMETER
                      );
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // Narrow.
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::SequenceProxyPushConsumer::_narrow (
        proxyconsumer.in () 
        TAO_ENV_ARG_PARAMETER
      );
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer_.in ()));

  proxy_consumer_->connect_sequence_push_supplier (supplier_ref.in ()
                                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Give ownership to POA.
  this->_remove_ref ();
}

void
TAO_Notify_SequencePushSupplier::disconnect (TAO_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_sequence_push_consumer (
                             TAO_ENV_SINGLE_ARG_PARAMETER
                           );
  ACE_CHECK;

  this->deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_SequencePushSupplier::subscription_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed */
    TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosNotifyComm::InvalidEventType))
{
  //No-Op.
}

PortableServer::POA_ptr
TAO_Notify_SequencePushSupplier::_default_POA (
    TAO_ENV_SINGLE_ARG_DECL_NOT_USED
  )
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_Notify_SequencePushSupplier::deactivate (TAO_ENV_SINGLE_ARG_DECL)
{
  PortableServer::POA_var poa =
        this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_SequencePushSupplier::disconnect_sequence_push_supplier (
        TAO_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  this->proxy_consumer_->disconnect_sequence_push_consumer (
                             TAO_ENV_SINGLE_ARG_PARAMETER
                           );
  ACE_CHECK;
  this->deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_SequencePushSupplier::send_events (
    const CosNotification::EventBatch & notifications 
    TAO_ENV_ARG_DECL
  )
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->push_structured_events (
                             notifications 
                             TAO_ENV_ARG_PARAMETER
                           );
  ACE_CHECK;
}
