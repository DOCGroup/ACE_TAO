// $Id$
#include "Notify_PushSupplier.h"

TAO_Notify_PushSupplier::TAO_Notify_PushSupplier (void)
{
  // No-Op
}

TAO_Notify_PushSupplier::~TAO_Notify_PushSupplier ()
{
}

void TAO_Notify_PushSupplier::init (PortableServer::POA_ptr poa, CORBA::Environment & /*ACE_TRY_ENV*/)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

void
TAO_Notify_PushSupplier::connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin, CORBA::Environment &ACE_TRY_ENV)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::PushSupplier_var objref = this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (CosNotifyChannelAdmin::ANY_EVENT, this->my_id_, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->consumer_proxy_ =
    CosNotifyChannelAdmin::ProxyPushConsumer::_narrow (proxyconsumer.in (),
                                                       ACE_TRY_ENV);
  ACE_CHECK;
  ACE_ASSERT (!CORBA::is_nil (consumer_proxy_.in ()));

  this->consumer_proxy_->connect_any_push_supplier (objref.in (),
                                                    ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_PushSupplier::send_event (const CORBA::Any & data, CORBA::Environment &ACE_TRY_ENV)
{
  this->consumer_proxy_->push (data, ACE_TRY_ENV);
}

void
TAO_Notify_PushSupplier::deactivate (CORBA::Environment &ACE_TRY_ENV)
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
    CosNotifyChannelAdmin::ProxyPushConsumer::_nil ();
}

void
TAO_Notify_PushSupplier::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  this->consumer_proxy_->disconnect_push_consumer (ACE_TRY_ENV);
  ACE_CHECK;
  this->deactivate (ACE_TRY_ENV);
}

PortableServer::POA_ptr
TAO_Notify_PushSupplier::_default_POA (CORBA::Environment& /* env */)
{
  return PortableServer::POA::_duplicate (this->default_POA_);
}

void
TAO_Notify_PushSupplier::subscription_change (const CosNotification::EventTypeSeq &/*added*/, const CosNotification::EventTypeSeq &/*removed*/, CORBA::Environment &/*ACE_TRY_ENV*/)
      ACE_THROW_SPEC ((
                       CORBA::SystemException,
                       CosNotifyComm::InvalidEventType
                       ))
{
  // TODO:
}

void
TAO_Notify_PushSupplier::disconnect_push_supplier (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->deactivate (ACE_TRY_ENV);
}
