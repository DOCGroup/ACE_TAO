// -*- C++ -*-
//
// $Id$

#include "orbsvcs/CosEvent/CEC_TypedConsumerAdmin.h"

#include "orbsvcs/CosEvent/CEC_TypedEventChannel.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_TypedConsumerAdmin.inl"
#endif /* __ACE_INLINE__ */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Implementation skeleton constructor
TAO_CEC_TypedConsumerAdmin::TAO_CEC_TypedConsumerAdmin (TAO_CEC_TypedEventChannel *ec)
  :  typed_event_channel_ (ec),
     typed_push_admin_ (ec)
{
  this->default_POA_ =
    this->typed_event_channel_->typed_consumer_poa ();
}

// Implementation skeleton destructor
TAO_CEC_TypedConsumerAdmin::~TAO_CEC_TypedConsumerAdmin (void)
{
}

void
TAO_CEC_TypedConsumerAdmin::invoke (const TAO_CEC_TypedEvent& typed_event)
{
  TAO_CEC_Propagate_Typed_Event typed_event_worker (typed_event, this->typed_event_channel_);

  this->typed_push_admin_.for_each (&typed_event_worker);
}

void
TAO_CEC_TypedConsumerAdmin::connected (TAO_CEC_ProxyPushSupplier *supplier)
{
  this->typed_push_admin_.connected (supplier);
}

void
TAO_CEC_TypedConsumerAdmin::reconnected (TAO_CEC_ProxyPushSupplier *supplier)
{
  this->typed_push_admin_.reconnected (supplier);
}

void
TAO_CEC_TypedConsumerAdmin::disconnected (TAO_CEC_ProxyPushSupplier *supplier)
{
  this->typed_push_admin_.disconnected (supplier);
}

void
TAO_CEC_TypedConsumerAdmin::shutdown (void)
{
  this->typed_push_admin_.shutdown ();
}

CosTypedEventChannelAdmin::TypedProxyPullSupplier_ptr
TAO_CEC_TypedConsumerAdmin::obtain_typed_pull_supplier (
    const char * /*supported_interface*/
  )
{
  throw CosTypedEventChannelAdmin::InterfaceNotSupported ();
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_CEC_TypedConsumerAdmin::obtain_typed_push_supplier (
    const char * uses_interface
  )

{
  // Register the consumer uses_interface with the EC
  int result = this->typed_event_channel_->consumer_register_uses_interace (uses_interface);

  if (result == -1)
    {
      throw CosTypedEventChannelAdmin::NoSuchImplementation ();
    }

  return this->typed_push_admin_.obtain ();
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_CEC_TypedConsumerAdmin::obtain_push_supplier (void)
{
    throw CORBA::NO_IMPLEMENT ();
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_CEC_TypedConsumerAdmin::obtain_pull_supplier (void)
{
    throw CORBA::NO_IMPLEMENT ();
}

PortableServer::POA_ptr
TAO_CEC_TypedConsumerAdmin::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

// ****************************************************************

void
TAO_CEC_Propagate_Typed_Event::work (TAO_CEC_ProxyPushSupplier *supplier)
{
  supplier->invoke (this->typed_event_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
