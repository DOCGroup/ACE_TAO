/* -*- C++ -*- $Id$ */
#include "Notify_StructuredPushConsumer.h"


// @@ Pradeep: in general i think that helper classes like this do
// more harm than help, my experience is that they can hardly cover
// but a few use cases, and increases confusion among users.
// It is better to put classes like this in examples and tests, so
// users can steal the code if they want to.  But then, i'm known to
// be a minimimalistic bigot ;-) ;-)

TAO_Notify_StructuredPushConsumer::TAO_Notify_StructuredPushConsumer (void)
{
  // No-Op.
}

TAO_Notify_StructuredPushConsumer::~TAO_Notify_StructuredPushConsumer (void)
{
}

// @@ Pradeep: do not assume that the user will activate the servant
// using _this() and changing the default POA, there are many other
// activation modes.
void TAO_Notify_StructuredPushConsumer::init (PortableServer::POA_ptr poa, CORBA::Environment & /*ACE_TRY_ENV*/)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

PortableServer::POA_ptr
TAO_Notify_StructuredPushConsumer::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->default_POA_);
}

void
TAO_Notify_StructuredPushConsumer::connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin, CORBA::Environment &ACE_TRY_ENV)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_ptr proxysupplier =
    consumer_admin->obtain_notification_push_supplier (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_id_, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxysupplier));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::
    _narrow (proxysupplier, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  proxy_supplier_->connect_structured_push_consumer (objref.in (),
                                                     ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_StructuredPushConsumer::deactivate (CORBA::Environment &ACE_TRY_ENV)
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

  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_nil ();
}

void
TAO_Notify_StructuredPushConsumer::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  this->proxy_supplier_->disconnect_structured_push_supplier(ACE_TRY_ENV);
  ACE_CHECK;

  this->deactivate (ACE_TRY_ENV);
}

void
TAO_Notify_StructuredPushConsumer::offer_change (const CosNotification::EventTypeSeq & /*added*/, const CosNotification::EventTypeSeq & /*removed*/, CORBA::Environment & //ACE_TRY_ENV
)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ))
{
  // No-Op.
}

void
TAO_Notify_StructuredPushConsumer::disconnect_structured_push_consumer (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->deactivate (ACE_TRY_ENV);
}
