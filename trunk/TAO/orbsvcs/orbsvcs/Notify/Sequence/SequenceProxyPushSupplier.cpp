// $Id$

#include "SequenceProxyPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "SequenceProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_SequenceProxyPushSupplier, "$id$")

#include "tao/debug.h"
#include "SequencePushConsumer.h"

TAO_NS_SequenceProxyPushSupplier::TAO_NS_SequenceProxyPushSupplier (void)
{
}

TAO_NS_SequenceProxyPushSupplier::~TAO_NS_SequenceProxyPushSupplier ()
{
}

void
TAO_NS_SequenceProxyPushSupplier::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "In TAO_NS_SequenceProxyPushConsumer::destroy \n"));

  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return;

  ACE_CHECK;
}

void
TAO_NS_SequenceProxyPushSupplier::release (void)
{
  if (this->consumer_)
    this->consumer_->release ();

  delete this;
  //@@ inform factory
}

void
TAO_NS_SequenceProxyPushSupplier::connect_sequence_push_consumer (CosNotifyComm::SequencePushConsumer_ptr push_consumer ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   , CosEventChannelAdmin::TypeError
                   ))
{
  // Convert Consumer to Base Type
  TAO_NS_SequencePushConsumer* consumer;
  ACE_NEW_THROW_EX (consumer,
                    TAO_NS_SequencePushConsumer (this),
                    CORBA::NO_MEMORY ());

  consumer->init (push_consumer, this->admin_properties_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->connect (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_SequenceProxyPushSupplier::disconnect_sequence_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxyType
TAO_NS_SequenceProxyPushSupplier::MyType (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::PUSH_SEQUENCE;
}
