// $Id$

#include "Notify_StructuredPushConsumer.h"

ACE_RCSID(Notify_Tests, Notify_StructuredPushConsumer, "$Id$")

TAO_Notify_StructuredPushConsumer::TAO_Notify_StructuredPushConsumer (void)
{
  // No-Op.
}

TAO_Notify_StructuredPushConsumer::~TAO_Notify_StructuredPushConsumer (void)
{
  // release all resources ...
  this->default_POA_ = PortableServer::POA::_nil ();

  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_nil ();
}

// @@ Pradeep: do not assume that the user will activate the servant
// using _this() and changing the default POA, there are many other
// activation modes.
// @@ Carlos: The <connect> method will activate the servant with the supplied
//            poa. This is the contract for clients of this class.
//
void TAO_Notify_StructuredPushConsumer::init
   (PortableServer::POA_ptr poa  ACE_ENV_ARG_DECL_NOT_USED)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

void
TAO_Notify_StructuredPushConsumer::name (ACE_CString& name)
{
  this->name_ = name;
}

const ACE_CString&
TAO_Notify_StructuredPushConsumer::name (void)
{
  return this->name_;
}

PortableServer::POA_ptr
TAO_Notify_StructuredPushConsumer::_default_POA (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}


CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr
TAO_Notify_StructuredPushConsumer::get_proxy_supplier (void)
{
  return proxy_supplier_.in ();
}

void
TAO_Notify_StructuredPushConsumer::connect (
    CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
    ACE_ENV_ARG_DECL
  )
{
  // Activate the consumer with the default_POA_.
  CosNotifyComm::StructuredPushConsumer_var consumer_ref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
                        CosNotifyChannelAdmin::STRUCTURED_EVENT,
                        proxy_supplier_id_
                        ACE_ENV_ARG_PARAMETER
                      );
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
        proxysupplier.in ()
        ACE_ENV_ARG_PARAMETER
      );
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  proxy_supplier_->connect_structured_push_consumer (consumer_ref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  // Give ownership to POA.
  this->_remove_ref ();
}

void
TAO_Notify_StructuredPushConsumer::connect (CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr proxy, CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
{
  // Activate the consumer with the default_POA_.
  PortableServer::ObjectId_var id =
    this->default_POA_->activate_object (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var object = this->default_POA_->id_to_reference (id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyComm::StructuredPushConsumer_var consumer_ref = CosNotifyComm::StructuredPushConsumer::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  proxy->connect_structured_push_consumer (consumer_ref.in ()
                                           ACE_ENV_ARG_PARAMETER);
  // Give ownership to POA.
  this->_remove_ref ();

    // save the proxy
  this->proxy_supplier_ = CosNotifyChannelAdmin::StructuredProxyPushSupplier::_duplicate (proxy);
  this->proxy_supplier_id_ = proxy_id;
}

void
TAO_Notify_StructuredPushConsumer::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  PortableServer::POA_var poa = this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_StructuredPushConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_supplier_->disconnect_structured_push_supplier(
                             ACE_ENV_SINGLE_ARG_PARAMETER
                           );
  ACE_CHECK;

  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_StructuredPushConsumer::offer_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed*/
    ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosNotifyComm::InvalidEventType))
{
  // No-Op.
}

void
TAO_Notify_StructuredPushConsumer::disconnect_structured_push_consumer
   (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_StructuredPushConsumer::push_structured_event
   (const CosNotification::StructuredEvent &/*notification*/
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosEventComm::Disconnected))
{
}
