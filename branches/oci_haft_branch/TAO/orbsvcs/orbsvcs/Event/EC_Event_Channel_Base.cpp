// $Id$

#include "EC_Event_Channel_Base.h"
#include "EC_Factory.h"
#include "EC_Dispatching.h"
#include "EC_ConsumerAdmin.h"
#include "EC_SupplierAdmin.h"
#include "EC_Timeout_Generator.h"
#include "EC_ObserverStrategy.h"
#include "EC_ConsumerControl.h"
#include "EC_SupplierControl.h"
#include "ace/Dynamic_Service.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Event_Channel_Base.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Event_Channel_Base, "$Id$")

TAO_EC_Event_Channel_Base::
TAO_EC_Event_Channel_Base (const TAO_EC_Event_Channel_Attributes& attr,
                           TAO_EC_Factory* factory,
                           int own_factory)
  : supplier_poa_ (PortableServer::POA::_duplicate (attr.supplier_poa)),
    consumer_poa_ (PortableServer::POA::_duplicate (attr.consumer_poa)),
    factory_ (factory),
    own_factory_ (own_factory),
    dispatching_ (0),
    filter_builder_ (0),
    supplier_filter_builder_ (0),
    consumer_admin_ (0),
    supplier_admin_ (0),
    timeout_generator_ (0),
    observer_strategy_ (0),
    scheduling_strategy_(0),
    consumer_reconnect_ (attr.consumer_reconnect),
    supplier_reconnect_ (attr.supplier_reconnect),
    disconnect_callbacks_ (attr.disconnect_callbacks),
    consumer_control_ (0),
    supplier_control_ (0),
    status_ (EC_S_IDLE)
{
  this->scheduler_ =
    CORBA::Object::_duplicate (attr.scheduler);
}

TAO_EC_Event_Channel_Base::~TAO_EC_Event_Channel_Base (void)
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

  this->factory_->destroy_scheduling_strategy (this->scheduling_strategy_);
  this->scheduling_strategy_ = 0;

  this->factory_->destroy_consumer_control (this->consumer_control_);
  this->consumer_control_ = 0;
  this->factory_->destroy_supplier_control (this->supplier_control_);
  this->supplier_control_ = 0;

  this->factory (0, 0);
}

void
TAO_EC_Event_Channel_Base::create_strategies (void)
{
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

  this->scheduling_strategy_ =
    this->factory_->create_scheduling_strategy (this);

  this->consumer_control_ =
    this->factory_->create_consumer_control (this);
  this->supplier_control_ =
    this->factory_->create_supplier_control (this);
}

void
TAO_EC_Event_Channel_Base::activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  {
    // First check if the EC is idle, if it is not then we need to
    // return right away...
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (this->status_ != EC_S_IDLE)
      return;
    this->status_ = EC_S_ACTIVATING;
  }
  this->dispatching_->activate ();
  this->timeout_generator_->activate ();
  this->consumer_control_->activate ();
  this->supplier_control_->activate ();
  {
    // Only when all the operations complete successfully we can mark
    // the EC as active...
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    ACE_ASSERT (this->status_ == EC_S_ACTIVATING);
    this->status_ = EC_S_ACTIVE;
  }
}

void
TAO_EC_Event_Channel_Base::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  {
    // First check if the EC is already active, if it is not then we
    // need to return right away...
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (this->status_ != EC_S_ACTIVE)
      return;
    this->status_ = EC_S_DESTROYING;
  }
  this->dispatching_->shutdown ();
  this->timeout_generator_->shutdown ();
  this->supplier_control_->shutdown ();
  this->consumer_control_->shutdown ();

  this->deactivate_supplier_admin ();
  this->deactivate_consumer_admin ();

  this->supplier_admin_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_admin_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  {
    // Wait until all the shutdown() operations return before marking
    // the EC as destroyed...
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    ACE_ASSERT (this->status_ == EC_S_DESTROYING);
    this->status_ = EC_S_DESTROYED;
  }
}

void
TAO_EC_Event_Channel_Base::deactivate_supplier_admin (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      PortableServer::POA_var supplier_poa =
        this->supplier_admin_->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::ObjectId_var supplier_id =
        supplier_poa->servant_to_id (this->supplier_admin_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_poa->deactivate_object (supplier_id.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
            // The deactivation can throw...
    }
  ACE_ENDTRY;
}

void
TAO_EC_Event_Channel_Base::deactivate_consumer_admin (void)
{
  ACE_TRY_NEW_ENV
    {
      PortableServer::POA_var consumer_poa =
        this->consumer_admin_->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::ObjectId_var consumer_id =
        consumer_poa->servant_to_id (this->consumer_admin_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      consumer_poa->deactivate_object (consumer_id.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
            // The deactivation can throw...
    }
  ACE_ENDTRY;
}

void
TAO_EC_Event_Channel_Base::connected (TAO_EC_ProxyPushConsumer* consumer
                                      ACE_ENV_ARG_DECL)
{
  this->consumer_admin_->peer_connected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->supplier_admin_->connected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->observer_strategy_->connected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_Event_Channel_Base::reconnected (TAO_EC_ProxyPushConsumer* consumer
                                        ACE_ENV_ARG_DECL)
{
  this->consumer_admin_->peer_reconnected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->supplier_admin_->reconnected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->observer_strategy_->connected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_Event_Channel_Base::disconnected (TAO_EC_ProxyPushConsumer* consumer
                                         ACE_ENV_ARG_DECL)
{
  this->consumer_admin_->peer_disconnected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->supplier_admin_->disconnected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->observer_strategy_->disconnected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_Event_Channel_Base::connected (TAO_EC_ProxyPushSupplier* supplier
                                      ACE_ENV_ARG_DECL)
{
  this->supplier_admin_->peer_connected (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->consumer_admin_->connected (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->observer_strategy_->connected (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_Event_Channel_Base::reconnected (TAO_EC_ProxyPushSupplier* supplier
                                        ACE_ENV_ARG_DECL)
{
  this->supplier_admin_->peer_reconnected (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->consumer_admin_->reconnected (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->observer_strategy_->connected (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_Event_Channel_Base::disconnected (TAO_EC_ProxyPushSupplier* supplier
                                         ACE_ENV_ARG_DECL)
{
  this->supplier_admin_->peer_disconnected (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->consumer_admin_->disconnected (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->observer_strategy_->disconnected (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

RtecEventChannelAdmin::ConsumerAdmin_ptr
TAO_EC_Event_Channel_Base::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->consumer_admin_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

RtecEventChannelAdmin::SupplierAdmin_ptr
TAO_EC_Event_Channel_Base::for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->supplier_admin_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_EC_Event_Channel_Base::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

RtecEventChannelAdmin::Observer_Handle
TAO_EC_Event_Channel_Base::append_observer (
       RtecEventChannelAdmin::Observer_ptr observer
       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER))
{
  return this->observer_strategy_->append_observer (observer
                                                    ACE_ENV_ARG_PARAMETER);
}

void
TAO_EC_Event_Channel_Base::remove_observer (
       RtecEventChannelAdmin::Observer_Handle handle
       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER))
{
  this->observer_strategy_->remove_observer (handle
                                             ACE_ENV_ARG_PARAMETER);
}

void
TAO_EC_Event_Channel_Base::for_each_consumer (
                    TAO_ESF_Worker<TAO_EC_ProxyPushSupplier> *worker
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->consumer_admin_->for_each (worker
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_Event_Channel_Base::for_each_supplier (
                    TAO_ESF_Worker<TAO_EC_ProxyPushConsumer> *worker
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->supplier_admin_->for_each (worker
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
