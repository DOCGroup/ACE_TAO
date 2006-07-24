// -*- C++ -*-
//
// $Id$

#include "orbsvcs/CosEvent/CEC_TypedConsumerAdmin.h"

#include "orbsvcs/CosEvent/CEC_TypedEventChannel.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_TypedConsumerAdmin.i"
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
TAO_CEC_TypedConsumerAdmin::invoke (const TAO_CEC_TypedEvent& typed_event
                                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_CEC_Propagate_Typed_Event typed_event_worker (typed_event, this->typed_event_channel_);

  this->typed_push_admin_.for_each (&typed_event_worker
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_TypedConsumerAdmin::connected (TAO_CEC_ProxyPushSupplier *supplier
                                       ACE_ENV_ARG_DECL)
{
  this->typed_push_admin_.connected (supplier ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_TypedConsumerAdmin::reconnected (TAO_CEC_ProxyPushSupplier *supplier
                                         ACE_ENV_ARG_DECL)
{
  this->typed_push_admin_.reconnected (supplier ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_TypedConsumerAdmin::disconnected (TAO_CEC_ProxyPushSupplier *supplier
                                          ACE_ENV_ARG_DECL)
{
  this->typed_push_admin_.disconnected (supplier ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_TypedConsumerAdmin::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->typed_push_admin_.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

CosTypedEventChannelAdmin::TypedProxyPullSupplier_ptr
TAO_CEC_TypedConsumerAdmin::obtain_typed_pull_supplier (
    const char * /*supported_interface*/
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosTypedEventChannelAdmin::InterfaceNotSupported
  ))
{
  ACE_THROW_RETURN (CosTypedEventChannelAdmin::InterfaceNotSupported (), 0);
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_CEC_TypedConsumerAdmin::obtain_typed_push_supplier (
    const char * uses_interface
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosTypedEventChannelAdmin::NoSuchImplementation
  ))

{
  // Register the consumer uses_interface with the EC
  int result = this->typed_event_channel_->consumer_register_uses_interace (uses_interface ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (result == -1)
    {
      ACE_THROW_RETURN (CosTypedEventChannelAdmin::NoSuchImplementation (), 0);
    }

  return this->typed_push_admin_.obtain (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_CEC_TypedConsumerAdmin::obtain_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_CEC_TypedConsumerAdmin::obtain_pull_supplier (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

PortableServer::POA_ptr
TAO_CEC_TypedConsumerAdmin::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

// ****************************************************************

void
TAO_CEC_Propagate_Typed_Event::work (TAO_CEC_ProxyPushSupplier *supplier
                                     ACE_ENV_ARG_DECL)
{
  supplier->invoke (this->typed_event_ ACE_ENV_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL
