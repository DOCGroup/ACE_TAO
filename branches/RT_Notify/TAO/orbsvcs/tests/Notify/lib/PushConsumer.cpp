// $Id$

#include "PushConsumer.h"

ACE_RCSID(Notify_Tests, Notify_PushConsumer, "$Id$")

TAO_NS_PushConsumer::TAO_NS_PushConsumer (void)
{
  // No-Op
}

TAO_NS_PushConsumer::~TAO_NS_PushConsumer ()
{
  // release all resources ...
  this->default_POA_ = PortableServer::POA::_nil ();

  this->supplier_proxy_ =
    CosNotifyChannelAdmin::ProxyPushSupplier::_nil ();
}

void TAO_NS_PushConsumer::init (PortableServer::POA_ptr poa
                                    ACE_ENV_ARG_DECL_NOT_USED)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}


PortableServer::POA_ptr
TAO_NS_PushConsumer::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_NS_PushConsumer::connect (
    CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
    ACE_ENV_ARG_DECL
  )
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::PushConsumer_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
                        CosNotifyChannelAdmin::ANY_EVENT,
                        this->proxy_id_
                        ACE_ENV_ARG_PARAMETER
                      );
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // Narrow.
  this->supplier_proxy_ =
    CosNotifyChannelAdmin::ProxyPushSupplier::_narrow (proxysupplier.in ()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_ASSERT (!CORBA::is_nil (supplier_proxy_.in ()));

  this->supplier_proxy_->connect_any_push_consumer (objref.in ()
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Give ownership to POA.
  this->_remove_ref ();
}

void
TAO_NS_PushConsumer::deactivate (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_NS_PushConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  this->supplier_proxy_->disconnect_push_supplier (
                             ACE_ENV_SINGLE_ARG_PARAMETER
                           );
  ACE_CHECK;

  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_PushConsumer::offer_change
   (const CosNotification::EventTypeSeq &/*added*/,
    const CosNotification::EventTypeSeq &/*removed*/
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosNotifyComm::InvalidEventType))
{
  // TODO:
}

void
TAO_NS_PushConsumer::push
   (const CORBA::Any & /*data*/
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosEventComm::Disconnected))
{
  // no op.
}

void
TAO_NS_PushConsumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}
