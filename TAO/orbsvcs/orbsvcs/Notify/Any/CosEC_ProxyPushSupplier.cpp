// $Id$

#include "CosEC_ProxyPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "CosEC_ProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_CosEC_ProxyPushSupplier, "$id$")

#include "tao/debug.h"
#include "PushConsumer.h"

TAO_NS_CosEC_ProxyPushSupplier::TAO_NS_CosEC_ProxyPushSupplier (void)
{
}

TAO_NS_CosEC_ProxyPushSupplier::~TAO_NS_CosEC_ProxyPushSupplier ()
{
}

void
TAO_NS_CosEC_ProxyPushSupplier::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "In TAO_NS_ProxyPushConsumer::destroy \n"));

  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return;

  ACE_CHECK;
}

void
TAO_NS_CosEC_ProxyPushSupplier::release (void)
{
  this->consumer_->release ();

  delete this;
  //@@ inform factory
}

void
TAO_NS_CosEC_ProxyPushSupplier::connect_push_consumer (CosEventComm::PushConsumer_ptr push_consumer
                                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected,
                   CosEventChannelAdmin::TypeError
                   ))
{
  // Convert Consumer to Base Type
  TAO_NS_PushConsumer* consumer;
  ACE_NEW_THROW_EX (consumer,
                    TAO_NS_PushConsumer (this),
                    CORBA::NO_MEMORY ());

  consumer->init (push_consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->connect (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_CosEC_ProxyPushSupplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
}
