// $Id$

#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_Dispatching.h"
#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event/EC_SupplierAdmin.h"
#include "orbsvcs/Event/EC_Timeout_Generator.h"
#include "orbsvcs/Event/EC_ObserverStrategy.h"
#include "orbsvcs/Event/EC_ConsumerControl.h"
#include "orbsvcs/Event/EC_SupplierControl.h"
#include "ace/Dynamic_Service.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Event_Channel_Base.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  // Destroy Strategies in the reverse order of creation, they
  // refere to each other during destruction and thus need to be
  // cleaned up properly.
  this->factory_->destroy_supplier_control (this->supplier_control_);
  this->supplier_control_ = 0;
  this->factory_->destroy_consumer_control (this->consumer_control_);
  this->consumer_control_ = 0;

  this->factory_->destroy_scheduling_strategy (this->scheduling_strategy_);
  this->scheduling_strategy_ = 0;

  this->factory_->destroy_observer_strategy (this->observer_strategy_);
  this->observer_strategy_ = 0;

  this->factory_->destroy_timeout_generator (this->timeout_generator_);
  this->timeout_generator_ = 0;

  this->factory_->destroy_supplier_admin (this->supplier_admin_);
  this->supplier_admin_ = 0;
  this->factory_->destroy_consumer_admin (this->consumer_admin_);
  this->consumer_admin_ = 0;

  this->factory_->destroy_supplier_filter_builder (this->supplier_filter_builder_);
  this->supplier_filter_builder_ = 0;

  this->factory_->destroy_filter_builder (this->filter_builder_);
  this->filter_builder_ = 0;

  this->factory_->destroy_dispatching (this->dispatching_);
  this->dispatching_ = 0;

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
TAO_EC_Event_Channel_Base::activate (void)
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
TAO_EC_Event_Channel_Base::shutdown (void)
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

  this->supplier_admin_->shutdown ();

  this->consumer_admin_->shutdown ();

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
  try
    {
      PortableServer::POA_var supplier_poa =
        this->supplier_admin_->_default_POA ();
      PortableServer::ObjectId_var supplier_id =
        supplier_poa->servant_to_id (this->supplier_admin_);
      supplier_poa->deactivate_object (supplier_id.in ());
    }
  catch (const CORBA::Exception&)
    {
            // The deactivation can throw...
    }
}

void
TAO_EC_Event_Channel_Base::deactivate_consumer_admin (void)
{
  try
    {
      PortableServer::POA_var consumer_poa =
        this->consumer_admin_->_default_POA ();
      PortableServer::ObjectId_var consumer_id =
        consumer_poa->servant_to_id (this->consumer_admin_);
      consumer_poa->deactivate_object (consumer_id.in ());
    }
  catch (const CORBA::Exception&)
    {
            // The deactivation can throw...
    }
}

void
TAO_EC_Event_Channel_Base::connected (TAO_EC_ProxyPushConsumer* consumer)
{
  this->consumer_admin_->peer_connected (consumer);
  this->supplier_admin_->connected (consumer);
  this->observer_strategy_->connected (consumer);
}

void
TAO_EC_Event_Channel_Base::reconnected (TAO_EC_ProxyPushConsumer* consumer)
{
  this->consumer_admin_->peer_reconnected (consumer);
  this->supplier_admin_->reconnected (consumer);
  this->observer_strategy_->connected (consumer);
}

void
TAO_EC_Event_Channel_Base::disconnected (TAO_EC_ProxyPushConsumer* consumer)
{
  this->consumer_admin_->peer_disconnected (consumer);
  this->supplier_admin_->disconnected (consumer);
  this->observer_strategy_->disconnected (consumer);
}

void
TAO_EC_Event_Channel_Base::connected (TAO_EC_ProxyPushSupplier* supplier)
{
  this->supplier_admin_->peer_connected (supplier);
  this->consumer_admin_->connected (supplier);
  this->observer_strategy_->connected (supplier);
}

void
TAO_EC_Event_Channel_Base::reconnected (TAO_EC_ProxyPushSupplier* supplier)
{
  this->supplier_admin_->peer_reconnected (supplier);
  this->consumer_admin_->reconnected (supplier);
  this->observer_strategy_->connected (supplier);
}

void
TAO_EC_Event_Channel_Base::disconnected (TAO_EC_ProxyPushSupplier* supplier)
{
  this->supplier_admin_->peer_disconnected (supplier);
  this->consumer_admin_->disconnected (supplier);
  this->observer_strategy_->disconnected (supplier);
}

RtecEventChannelAdmin::ConsumerAdmin_ptr
TAO_EC_Event_Channel_Base::for_consumers (void)
{
  return this->consumer_admin_->_this ();
}

RtecEventChannelAdmin::SupplierAdmin_ptr
TAO_EC_Event_Channel_Base::for_suppliers (void)
{
  return this->supplier_admin_->_this ();
}

void
TAO_EC_Event_Channel_Base::destroy (void)
{
  this->shutdown ();
}

RtecEventChannelAdmin::Observer_Handle
TAO_EC_Event_Channel_Base::append_observer (
       RtecEventChannelAdmin::Observer_ptr observer)
{
  return this->observer_strategy_->append_observer (observer);
}

void
TAO_EC_Event_Channel_Base::remove_observer (
       RtecEventChannelAdmin::Observer_Handle handle)
{
  this->observer_strategy_->remove_observer (handle);
}

void
TAO_EC_Event_Channel_Base::for_each_consumer (
                    TAO_ESF_Worker<TAO_EC_ProxyPushSupplier> *worker)
{
  this->consumer_admin_->for_each (worker);
}

void
TAO_EC_Event_Channel_Base::for_each_supplier (
                    TAO_ESF_Worker<TAO_EC_ProxyPushConsumer> *worker)
{
  this->supplier_admin_->for_each (worker);
}

TAO_END_VERSIONED_NAMESPACE_DECL
