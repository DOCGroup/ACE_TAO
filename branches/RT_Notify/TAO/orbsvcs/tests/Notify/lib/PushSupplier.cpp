// $Id$

#include "PushSupplier.h"

ACE_RCSID(Notify_Tests, Notify_PushSupplier, "$Id$")

TAO_NS_PushSupplier::TAO_NS_PushSupplier (void)
{
  // No-Op
}

TAO_NS_PushSupplier::~TAO_NS_PushSupplier ()
{
  // release all resources ...
  this->default_POA_ = PortableServer::POA::_nil ();

  this->consumer_proxy_ =
    CosNotifyChannelAdmin::ProxyPushConsumer::_nil ();
}

void TAO_NS_PushSupplier::init
   (PortableServer::POA_ptr poa  ACE_ENV_ARG_DECL_NOT_USED)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

void
TAO_NS_PushSupplier::connect (
    CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin
    ACE_ENV_ARG_DECL
  )
{
  // Activate the consumer with the default_POA_.
  CosNotifyComm::PushSupplier_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (
                        CosNotifyChannelAdmin::ANY_EVENT,
                        this->my_id_
                        ACE_ENV_ARG_PARAMETER
                      );
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // Narrow.
  this->consumer_proxy_ =
    CosNotifyChannelAdmin::ProxyPushConsumer::_narrow (proxyconsumer.in ()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_ASSERT (!CORBA::is_nil (consumer_proxy_.in ()));

  this->consumer_proxy_->connect_any_push_supplier (objref.in ()
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Give ownership to POA.
  this->_remove_ref ();
}

void
TAO_NS_PushSupplier::send_event (const CORBA::Any & data ACE_ENV_ARG_DECL)
{
  this->consumer_proxy_->push (data ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_PushSupplier::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_PushSupplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  this->consumer_proxy_->disconnect_push_consumer (
                             ACE_ENV_SINGLE_ARG_PARAMETER
                           );
  ACE_CHECK;
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

PortableServer::POA_ptr
TAO_NS_PushSupplier::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_NS_PushSupplier::subscription_change
   (const CosNotification::EventTypeSeq &/*added*/,
    const CosNotification::EventTypeSeq &/*removed*/
    ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosNotifyComm::InvalidEventType))
{
  // TODO:
}

void
TAO_NS_PushSupplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}
