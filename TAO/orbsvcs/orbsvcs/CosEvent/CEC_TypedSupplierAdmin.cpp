// -*- C++ -*-
//
// $Id$

#include "CEC_TypedSupplierAdmin.h"

#include "CEC_TypedEventChannel.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_TypedSupplierAdmin.i"
#endif /* __ACE_INLINE__ */

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
TAO_CEC_TypedSupplierAdmin::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_TypedSupplierAdmin::connected (TAO_CEC_TypedProxyPushConsumer *consumer
                                       ACE_ENV_ARG_DECL)
{
  this->typed_push_admin_.connected (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_TypedSupplierAdmin::reconnected (TAO_CEC_TypedProxyPushConsumer *consumer
                                         ACE_ENV_ARG_DECL)
{
  this->typed_push_admin_.reconnected (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_TypedSupplierAdmin::disconnected (TAO_CEC_TypedProxyPushConsumer *consumer
                                          ACE_ENV_ARG_DECL)
{
  this->typed_push_admin_.disconnected (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_TypedSupplierAdmin::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->typed_push_admin_.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

CosTypedEventChannelAdmin::TypedProxyPushConsumer_ptr
TAO_CEC_TypedSupplierAdmin::obtain_typed_push_consumer (
    const char * supported_interface
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosTypedEventChannelAdmin::InterfaceNotSupported
  ))

{
  int result = this->typed_event_channel_->supplier_register_supported_interface (supported_interface ACE_ENV_ARG_PARAMETER);

  if (result == -1)
    {
      ACE_THROW (CosTypedEventChannelAdmin::InterfaceNotSupported ());
    }

  return this->typed_push_admin_.obtain (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_CEC_TypedSupplierAdmin::obtain_typed_pull_consumer (
    const char * /* uses_interface */
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosTypedEventChannelAdmin::NoSuchImplementation
  ))

{
  ACE_THROW (CosTypedEventChannelAdmin::NoSuchImplementation ());
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_CEC_TypedSupplierAdmin::obtain_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_CEC_TypedSupplierAdmin::obtain_pull_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_ESF_Proxy_Admin<TAO_CEC_TypedEventChannel,TAO_CEC_TypedProxyPushConsumer,CosTypedEventChannelAdmin::TypedProxyPushConsumer>;
template class TAO_ESF_Shutdown_Proxy<TAO_CEC_TypedProxyPushConsumer>;
template class TAO_ESF_Worker<TAO_CEC_TypedProxyPushConsumer>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_ESF_Proxy_Admin<TAO_CEC_TypedEventChannel,TAO_CEC_TypedProxyPushConsumer,CosTypedEventChannelAdmin::TypedProxyPushConsumer>
#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_CEC_TypedProxyPushConsumer>
#pragma instantiate TAO_ESF_Worker<TAO_CEC_TypedProxyPushConsumer>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

