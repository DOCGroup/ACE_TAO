// $Id$

#include "CosEC_ProxyPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "CosEC_ProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Notify, TAO_Notify_CosEC_ProxyPushSupplier, "$Id$")

#include "tao/debug.h"
#include "PushConsumer.h"

TAO_Notify_CosEC_ProxyPushSupplier::TAO_Notify_CosEC_ProxyPushSupplier (void)
{
}

TAO_Notify_CosEC_ProxyPushSupplier::~TAO_Notify_CosEC_ProxyPushSupplier ()
{
}

void
TAO_Notify_CosEC_ProxyPushSupplier::release (void)
{
  this->consumer_->release ();

  delete this;
  //@@ inform factory
}

void
TAO_Notify_CosEC_ProxyPushSupplier::connect_push_consumer (CosEventComm::PushConsumer_ptr push_consumer
                                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected,
                   CosEventChannelAdmin::TypeError
                   ))
{
  // Convert Consumer to Base Type
  TAO_Notify_PushConsumer* consumer;
  ACE_NEW_THROW_EX (consumer,
                    TAO_Notify_PushConsumer (this),
                    CORBA::NO_MEMORY ());

  consumer->init (push_consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->connect (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_CosEC_ProxyPushSupplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
}

const char *
TAO_Notify_CosEC_ProxyPushSupplier::get_proxy_type_name (void) const
{
  return "ec_proxy_push_supplier";
}
