// $Id$

#include "CEC_EventChannel.h"
#include "CEC_Dispatching.h"
#include "CEC_Pulling_Strategy.h"
#include "CEC_ConsumerAdmin.h"
#include "CEC_SupplierAdmin.h"
#include "CEC_ConsumerControl.h"
#include "CEC_SupplierControl.h"
#include "ace/Dynamic_Service.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_EventChannel.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_EventChannel, "$Id$")

TAO_CEC_EventChannel::
TAO_CEC_EventChannel (const TAO_CEC_EventChannel_Attributes& attr,
                      TAO_CEC_Factory* factory,
                      int own_factory)
  : supplier_poa_ (PortableServer::POA::_duplicate (attr.supplier_poa)),
    consumer_poa_ (PortableServer::POA::_duplicate (attr.consumer_poa)),
    factory_ (factory),
    own_factory_ (own_factory),
    consumer_reconnect_ (attr.consumer_reconnect),
    supplier_reconnect_ (attr.supplier_reconnect),
    disconnect_callbacks_ (attr.disconnect_callbacks),
    busy_hwm_ (attr.busy_hwm),
    max_write_delay_ (attr.max_write_delay)
{
  if (this->factory_ == 0)
    {
      this->factory_ =
        ACE_Dynamic_Service<TAO_CEC_Factory>::instance ("CEC_Factory");
      this->own_factory_ = 0;
      ACE_ASSERT (this->factory_ != 0);
    }

  this->dispatching_ =
    this->factory_->create_dispatching (this);
  this->pulling_strategy_ =
    this->factory_->create_pulling_strategy (this);
  this->consumer_admin_ =
    this->factory_->create_consumer_admin (this);
  this->supplier_admin_ =
    this->factory_->create_supplier_admin (this);
  this->consumer_control_ =
    this->factory_->create_consumer_control (this);
  this->supplier_control_ =
    this->factory_->create_supplier_control (this);
}

TAO_CEC_EventChannel::~TAO_CEC_EventChannel (void)
{
  this->factory_->destroy_dispatching (this->dispatching_);
  this->dispatching_ = 0;

  this->factory_->destroy_pulling_strategy (this->pulling_strategy_);
  this->pulling_strategy_ = 0;

  this->factory_->destroy_consumer_admin (this->consumer_admin_);
  this->consumer_admin_ = 0;
  this->factory_->destroy_supplier_admin (this->supplier_admin_);
  this->supplier_admin_ = 0;

  this->factory_->destroy_consumer_control (this->consumer_control_);
  this->consumer_control_ = 0;
  this->factory_->destroy_supplier_control (this->supplier_control_);
  this->supplier_control_ = 0;

  if (this->own_factory_)
    delete this->factory_;
}

void
TAO_CEC_EventChannel::activate (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->dispatching_->activate ();
  this->pulling_strategy_->activate ();
  this->consumer_control_->activate ();
  this->supplier_control_->activate ();
}

void
TAO_CEC_EventChannel::shutdown (TAO_ENV_SINGLE_ARG_DECL)
{
  this->dispatching_->shutdown ();
  this->pulling_strategy_->shutdown ();
  this->supplier_control_->shutdown ();
  this->consumer_control_->shutdown ();

  PortableServer::POA_var consumer_poa =
    this->consumer_admin_->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var consumer_id =
    consumer_poa->servant_to_id (this->consumer_admin_ TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  consumer_poa->deactivate_object (consumer_id.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POA_var supplier_poa =
    this->supplier_admin_->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var supplier_id =
    supplier_poa->servant_to_id (this->supplier_admin_ TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  supplier_poa->deactivate_object (supplier_id.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_admin_->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);

  this->consumer_admin_->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_CEC_EventChannel::connected (TAO_CEC_ProxyPushConsumer* consumer
                                 TAO_ENV_ARG_DECL)
{
  this->supplier_admin_->connected (consumer TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_EventChannel::reconnected (TAO_CEC_ProxyPushConsumer* consumer
                                   TAO_ENV_ARG_DECL)
{
  this->supplier_admin_->reconnected (consumer TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_EventChannel::disconnected (TAO_CEC_ProxyPushConsumer* consumer
                                    TAO_ENV_ARG_DECL)
{
  this->supplier_admin_->disconnected (consumer TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_EventChannel::connected (TAO_CEC_ProxyPullConsumer* consumer
                                 TAO_ENV_ARG_DECL)
{
  this->supplier_admin_->connected (consumer TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_EventChannel::reconnected (TAO_CEC_ProxyPullConsumer* consumer
                                   TAO_ENV_ARG_DECL)
{
  this->supplier_admin_->reconnected (consumer TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_EventChannel::disconnected (TAO_CEC_ProxyPullConsumer* consumer
                                    TAO_ENV_ARG_DECL)
{
  this->supplier_admin_->disconnected (consumer TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_EventChannel::connected (TAO_CEC_ProxyPushSupplier* supplier
                                 TAO_ENV_ARG_DECL)
{
  this->consumer_admin_->connected (supplier TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_EventChannel::reconnected (TAO_CEC_ProxyPushSupplier* supplier
                                   TAO_ENV_ARG_DECL)
{
  this->consumer_admin_->reconnected (supplier TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_EventChannel::disconnected (TAO_CEC_ProxyPushSupplier* supplier
                                    TAO_ENV_ARG_DECL)
{
  this->consumer_admin_->disconnected (supplier TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_EventChannel::connected (TAO_CEC_ProxyPullSupplier* supplier
                                 TAO_ENV_ARG_DECL)
{
  this->consumer_admin_->connected (supplier TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_EventChannel::reconnected (TAO_CEC_ProxyPullSupplier* supplier
                                   TAO_ENV_ARG_DECL)
{
  this->consumer_admin_->reconnected (supplier TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_EventChannel::disconnected (TAO_CEC_ProxyPullSupplier* supplier
                                    TAO_ENV_ARG_DECL)
{
  this->consumer_admin_->disconnected (supplier TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_CEC_EventChannel::for_consumers (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->consumer_admin_->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_CEC_EventChannel::for_suppliers (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->supplier_admin_->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_CEC_EventChannel::destroy (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
}
