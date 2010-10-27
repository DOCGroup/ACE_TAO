// $Id$

#include "orbsvcs/CosEvent/CEC_TypedSupplierAdmin.h"

#include "orbsvcs/CosEvent/CEC_TypedEventChannel.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_TypedSupplierAdmin.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Implementation skeleton constructor
TAO_CEC_TypedSupplierAdmin::TAO_CEC_TypedSupplierAdmin (TAO_CEC_TypedEventChannel *ec)
  :  typed_event_channel_ (ec),
     typed_push_admin_ (ec)
{
  this->default_POA_ =
    this->typed_event_channel_->typed_supplier_poa ();
}

// Implementation skeleton destructor
TAO_CEC_TypedSupplierAdmin::~TAO_CEC_TypedSupplierAdmin (void)
{
}

PortableServer::POA_ptr
TAO_CEC_TypedSupplierAdmin::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_TypedSupplierAdmin::connected (TAO_CEC_TypedProxyPushConsumer *consumer)
{
  this->typed_push_admin_.connected (consumer);
}

void
TAO_CEC_TypedSupplierAdmin::reconnected (TAO_CEC_TypedProxyPushConsumer *consumer)
{
  this->typed_push_admin_.reconnected (consumer);
}

void
TAO_CEC_TypedSupplierAdmin::disconnected (TAO_CEC_TypedProxyPushConsumer *consumer)
{
  this->typed_push_admin_.disconnected (consumer);
}

void
TAO_CEC_TypedSupplierAdmin::shutdown (void)
{
  this->typed_push_admin_.shutdown ();
}

CosTypedEventChannelAdmin::TypedProxyPushConsumer_ptr
TAO_CEC_TypedSupplierAdmin::obtain_typed_push_consumer (
    const char * supported_interface
  )

{
  int result = this->typed_event_channel_->supplier_register_supported_interface (supported_interface);

  if (result == -1)
    {
      throw CosTypedEventChannelAdmin::InterfaceNotSupported ();
    }

  return this->typed_push_admin_.obtain ();
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_CEC_TypedSupplierAdmin::obtain_typed_pull_consumer (
    const char * /* uses_interface */
  )

{
  throw CosTypedEventChannelAdmin::NoSuchImplementation ();
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_CEC_TypedSupplierAdmin::obtain_push_consumer (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_CEC_TypedSupplierAdmin::obtain_pull_consumer (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
