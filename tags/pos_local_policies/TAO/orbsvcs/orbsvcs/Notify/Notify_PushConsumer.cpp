// $Id$
#include "Notify_PushConsumer.h"

TAO_Notify_PushConsumer::TAO_Notify_PushConsumer (void)
{
  // No-Op
}

TAO_Notify_PushConsumer::~TAO_Notify_PushConsumer ()
{
  // No-Op.
}

void TAO_Notify_PushConsumer::init (PortableServer::POA_ptr poa, CORBA::Environment & /*ACE_TRY_ENV*/)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}


PortableServer::POA_ptr
TAO_Notify_PushConsumer::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->default_POA_);
}

void
TAO_Notify_PushConsumer::connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin, CORBA::Environment &ACE_TRY_ENV)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::PushConsumer_var objref = this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (CosNotifyChannelAdmin::ANY_EVENT, this->proxy_id_, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->supplier_proxy_ =
    CosNotifyChannelAdmin::ProxyPushSupplier::_narrow (proxysupplier.in (),
                                                       ACE_TRY_ENV);
  ACE_CHECK;
  ACE_ASSERT (!CORBA::is_nil (supplier_proxy_.in ()));

  this->supplier_proxy_->connect_any_push_consumer (objref.in (),
                                                    ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_PushConsumer::deactivate (CORBA::Environment &ACE_TRY_ENV)
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

  this->supplier_proxy_ =
    CosNotifyChannelAdmin::ProxyPushSupplier::_nil ();
}

void
TAO_Notify_PushConsumer::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  this->supplier_proxy_->disconnect_push_supplier (ACE_TRY_ENV);
  ACE_CHECK;

  this->deactivate (ACE_TRY_ENV);
}

void
TAO_Notify_PushConsumer::offer_change (const CosNotification::EventTypeSeq &/*added*/, const CosNotification::EventTypeSeq &/*removed*/, CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  // TODO:
}

void
TAO_Notify_PushConsumer::disconnect_push_consumer (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->deactivate (ACE_TRY_ENV);
}
