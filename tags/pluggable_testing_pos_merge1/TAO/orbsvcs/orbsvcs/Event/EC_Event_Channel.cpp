// $Id$

#include "EC_Event_Channel.h"
#include "EC_Default_Factory.h"
#include "EC_Dispatching.h"
#include "EC_ConsumerAdmin.h"
#include "EC_SupplierAdmin.h"
#include "EC_Timeout_Generator.h"
#include "EC_ObserverStrategy.h"
#include "ace/Dynamic_Service.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Event_Channel.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Event_Channel, "$Id$")

TAO_EC_Event_Channel::
TAO_EC_Event_Channel (const TAO_EC_Event_Channel_Attributes& attr,
                      TAO_EC_Factory* factory,
                      int own_factory)
  : supplier_poa_ (PortableServer::POA::_duplicate (attr.supplier_poa)),
    consumer_poa_ (PortableServer::POA::_duplicate (attr.consumer_poa)),
    factory_ (factory),
    own_factory_ (own_factory),
    consumer_reconnect_ (attr.consumer_reconnect),
    supplier_reconnect_ (attr.supplier_reconnect),
    busy_hwm_ (attr.busy_hwm),
    max_write_delay_ (attr.max_write_delay)
{
  if (this->factory_ == 0)
    {
      this->factory_ =
        ACE_Dynamic_Service<TAO_EC_Factory>::instance ("EC_Factory");
      this->own_factory_ = 0;

      if (this->factory_ == 0)
        {
          ACE_NEW (this->factory_,
                   TAO_EC_Default_Factory);
          this->own_factory_ = 1;
        }
    }

  this->dispatching_ =
    this->factory_->create_dispatching (this);
  this->filter_builder_ =
    this->factory_->create_filter_builder (this);
  this->supplier_filter_builder_ =
    this->factory_->create_supplier_filter_builder (this);
  this->consumer_admin_ =
    this->factory_->create_consumer_admin (this);
  this->supplier_admin_ =
    this->factory_->create_supplier_admin (this);
  this->timeout_generator_ =
    this->factory_->create_timeout_generator (this);
  this->observer_strategy_ =
    this->factory_->create_observer_strategy (this);

  this->scheduler_ =
    RtecScheduler::Scheduler::_duplicate (attr.scheduler);

  this->scheduling_strategy_ =
    this->factory_->create_scheduling_strategy (this);
}

TAO_EC_Event_Channel::~TAO_EC_Event_Channel (void)
{
  this->factory_->destroy_dispatching (this->dispatching_);
  this->dispatching_ = 0;
  this->factory_->destroy_filter_builder (this->filter_builder_);
  this->filter_builder_ = 0;
  this->factory_->destroy_supplier_filter_builder (this->supplier_filter_builder_);
  this->supplier_filter_builder_ = 0;
  this->factory_->destroy_consumer_admin (this->consumer_admin_);
  this->consumer_admin_ = 0;
  this->factory_->destroy_supplier_admin (this->supplier_admin_);
  this->supplier_admin_ = 0;
  this->factory_->destroy_timeout_generator (this->timeout_generator_);
  this->timeout_generator_ = 0;
  this->factory_->destroy_observer_strategy (this->observer_strategy_);
  this->observer_strategy_ = 0;

  if (this->own_factory_)
    delete this->factory_;
}

void
TAO_EC_Event_Channel::activate (CORBA::Environment&)
{
  this->dispatching_->activate ();
  this->timeout_generator_->activate ();
}

void
TAO_EC_Event_Channel::shutdown (CORBA::Environment& ACE_TRY_ENV)
{
  this->dispatching_->shutdown ();
  this->timeout_generator_->shutdown ();

  PortableServer::POA_var consumer_poa =
    this->consumer_admin_->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var consumer_id =
    consumer_poa->servant_to_id (this->consumer_admin_, ACE_TRY_ENV);
  ACE_CHECK;
  consumer_poa->deactivate_object (consumer_id.in (), ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POA_var supplier_poa =
    this->supplier_admin_->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var supplier_id =
    supplier_poa->servant_to_id (this->supplier_admin_, ACE_TRY_ENV);
  ACE_CHECK;
  supplier_poa->deactivate_object (supplier_id.in (), ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_admin_->shutdown (ACE_TRY_ENV);

  this->consumer_admin_->shutdown (ACE_TRY_ENV);
}

void
TAO_EC_Event_Channel::connected (TAO_EC_ProxyPushConsumer* consumer,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  this->consumer_admin_->connected (consumer, ACE_TRY_ENV);
  ACE_CHECK;
  this->supplier_admin_->connected (consumer, ACE_TRY_ENV);
  ACE_CHECK;
  this->observer_strategy_->connected (consumer, ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_EC_Event_Channel::disconnected (TAO_EC_ProxyPushConsumer* consumer,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  this->consumer_admin_->disconnected (consumer, ACE_TRY_ENV);
  ACE_CHECK;
  this->supplier_admin_->disconnected (consumer, ACE_TRY_ENV);
  ACE_CHECK;
  this->observer_strategy_->disconnected (consumer, ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_EC_Event_Channel::connected (TAO_EC_ProxyPushSupplier* supplier,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  this->supplier_admin_->connected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
  this->consumer_admin_->connected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
  this->observer_strategy_->connected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_EC_Event_Channel::disconnected (TAO_EC_ProxyPushSupplier* supplier,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  this->supplier_admin_->disconnected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
  this->consumer_admin_->disconnected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
  this->observer_strategy_->disconnected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
}

RtecEventChannelAdmin::ConsumerAdmin_ptr
TAO_EC_Event_Channel::for_consumers (CORBA::Environment& ACE_TRY_ENV)
{
  return this->consumer_admin_->_this (ACE_TRY_ENV);
}

RtecEventChannelAdmin::SupplierAdmin_ptr
TAO_EC_Event_Channel::for_suppliers (CORBA::Environment& ACE_TRY_ENV)
{
  return this->supplier_admin_->_this (ACE_TRY_ENV);
}

void
TAO_EC_Event_Channel::destroy (CORBA::Environment &TAO_TRY_ENV)
{
  this->shutdown (TAO_TRY_ENV);
}

RtecEventChannelAdmin::Observer_Handle
TAO_EC_Event_Channel::append_observer (
       RtecEventChannelAdmin::Observer_ptr observer,
       CORBA::Environment &ACE_IN_ENV)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                     RtecEventChannel::EventChannel::CANT_APPEND_OBSERVER))
{
  return this->observer_strategy_->append_observer (observer, ACE_IN_ENV);
}

void
TAO_EC_Event_Channel::remove_observer (
       RtecEventChannelAdmin::Observer_Handle handle,
       CORBA::Environment &ACE_IN_ENV)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                     RtecEventChannel::EventChannel::CANT_REMOVE_OBSERVER))
{
  this->observer_strategy_->remove_observer (handle, ACE_IN_ENV);
}
