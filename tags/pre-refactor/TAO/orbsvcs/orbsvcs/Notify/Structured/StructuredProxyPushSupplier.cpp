// $Id$

#include "StructuredProxyPushSupplier.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/debug.h"

#include "StructuredPushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_StructuredProxyPushSupplier, "$Id$")

TAO_NS_StructuredProxyPushSupplier::TAO_NS_StructuredProxyPushSupplier (void)
{
}

TAO_NS_StructuredProxyPushSupplier::~TAO_NS_StructuredProxyPushSupplier ()
{
}

void
TAO_NS_StructuredProxyPushSupplier::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "In TAO_NS_StructuredProxyPushConsumer::destroy \n"));

  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return;

  ACE_CHECK;
}

void
TAO_NS_StructuredProxyPushSupplier::release (void)
{
  if (this->consumer_)
    this->consumer_->release ();

  delete this;
  //@@ inform factory
}

CosNotifyChannelAdmin::ProxyType
TAO_NS_StructuredProxyPushSupplier::MyType (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::PUSH_STRUCTURED;
}

void
TAO_NS_StructuredProxyPushSupplier::connect_structured_push_consumer (CosNotifyComm::StructuredPushConsumer_ptr push_consumer ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   , CosEventChannelAdmin::TypeError
                   ))
{
  // Convert Consumer to Base Type
  TAO_NS_StructuredPushConsumer* consumer;
  ACE_NEW_THROW_EX (consumer,
                    TAO_NS_StructuredPushConsumer (this),
                    CORBA::NO_MEMORY ());

  consumer->init (push_consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->connect (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_StructuredProxyPushSupplier::disconnect_structured_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
}
