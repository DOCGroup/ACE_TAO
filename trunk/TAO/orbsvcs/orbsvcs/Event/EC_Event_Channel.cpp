// $Id$

#include "EC_Event_Channel.h"
#include "EC_Factory.h"
#include "EC_Dispatching.h"
#include "EC_ConsumerAdmin.h"
#include "EC_SupplierAdmin.h"
#include "Timer_Module.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Event_Channel.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Event_Channel, "$Id$")

TAO_EC_Event_Channel::TAO_EC_Event_Channel (TAO_EC_Factory* factory)
  : factory_ (factory)
{
  ACE_ASSERT (this->factory_ != 0);

  this->dispatching_ = 
    this->factory_->create_dispatching (this);
  this->filter_builder_ = 
    this->factory_->create_filter_builder (this);
  this->consumer_admin_ =
    this->factory_->create_consumer_admin (this);
  this->supplier_admin_ = 
    this->factory_->create_supplier_admin (this);
  this->timer_module_ = 
    this->factory_->create_timer_module (this);
}

TAO_EC_Event_Channel::~TAO_EC_Event_Channel (void)
{
  this->factory_->destroy_dispatching (this->dispatching_);
  this->dispatching_ = 0;
  this->factory_->destroy_filter_builder (this->filter_builder_);
  this->filter_builder_ = 0;
  this->factory_->destroy_consumer_admin (this->consumer_admin_);
  this->consumer_admin_ = 0;
  this->factory_->destroy_supplier_admin (this->supplier_admin_);
  this->supplier_admin_ = 0;
  this->factory_->destroy_timer_module (this->timer_module_);
  this->timer_module_ = 0;
}

void
TAO_EC_Event_Channel::activate (CORBA::Environment& ACE_TRY_ENV)
{
  this->dispatching_->activate ();
  this->timer_module_->activate ();
  
  PortableServer::POA_var supplier_poa =
    this->factory_->supplier_poa (ACE_TRY_ENV);
  ACE_CHECK;
  this->supplier_admin_->set_default_POA (supplier_poa.in ());

  PortableServer::POA_var consumer_poa =
    this->factory_->consumer_poa (ACE_TRY_ENV);
  ACE_CHECK;
  this->consumer_admin_->set_default_POA (consumer_poa.in ());
}

void
TAO_EC_Event_Channel::shutdown (CORBA::Environment& ACE_TRY_ENV)
{
  this->dispatching_->shutdown ();
  this->timer_module_->shutdown ();

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
}

void
TAO_EC_Event_Channel::connected (TAO_EC_ProxyPushConsumer* consumer,
				 CORBA::Environment &ACE_TRY_ENV)
{
  this->consumer_admin_->connected (consumer, ACE_TRY_ENV);
  ACE_CHECK;
  this->supplier_admin_->connected (consumer, ACE_TRY_ENV);
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
}

void
TAO_EC_Event_Channel::connected (TAO_EC_ProxyPushSupplier* supplier,
				 CORBA::Environment &ACE_TRY_ENV)
{
  this->supplier_admin_->connected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
  this->consumer_admin_->connected (supplier, ACE_TRY_ENV);
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
TAO_EC_Event_Channel::append_observer (RtecEventChannelAdmin::Observer_ptr,
                                       CORBA::Environment &)
{
  // @@ TODO
  return 0;
}

void
TAO_EC_Event_Channel::remove_observer (RtecEventChannelAdmin::Observer_Handle,
                                       CORBA::Environment &)
{
}

