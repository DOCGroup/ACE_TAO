// -*- C++ -*-
// $Id$

#include "Notify_StructuredPushSupplier.h"

ACE_RCSID(Notify_Tests, Notify_StructuredPushSupplier, "$Id$")

TAO_Notify_StructuredPushSupplier::TAO_Notify_StructuredPushSupplier (void)
{
}

TAO_Notify_StructuredPushSupplier::~TAO_Notify_StructuredPushSupplier ()
{
  // release all resources ...
  this->default_POA_ = PortableServer::POA::_nil ();

  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_nil ();
}

void
TAO_Notify_StructuredPushSupplier::init
   (PortableServer::POA_ptr poa  TAO_ENV_ARG_DECL_NOT_USED)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr
TAO_Notify_StructuredPushSupplier::get_proxy_consumer (void)
{
  return proxy_consumer_.in ();
}

void
TAO_Notify_StructuredPushSupplier::connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin TAO_ENV_ARG_DECL)
{
  CosNotifyComm::StructuredPushSupplier_var supplier_ref =
    this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_consumer_id_ TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxyconsumer.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer_.in ()));

  proxy_consumer_->connect_structured_push_supplier (supplier_ref.in ()
                                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // give ownership to POA
  this->_remove_ref ();
}

void
TAO_Notify_StructuredPushSupplier::disconnect (TAO_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer(TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_StructuredPushSupplier::subscription_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed */
    TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ))
{
  //No-Op.
}

PortableServer::POA_ptr
TAO_Notify_StructuredPushSupplier::_default_POA (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /* env */)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_Notify_StructuredPushSupplier::deactivate (TAO_ENV_SINGLE_ARG_DECL)
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
TAO_Notify_StructuredPushSupplier::disconnect_structured_push_supplier (
        TAO_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  this->proxy_consumer_->disconnect_structured_push_consumer (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_StructuredPushSupplier::send_event (const CosNotification::StructuredEvent& event TAO_ENV_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->push_structured_event (event TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
