// $Id$

#include "SequenceProxyPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "SequenceProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Notify, TAO_Notify_SequenceProxyPushSupplier, "$Id$")

#include "tao/debug.h"
#include "SequencePushConsumer.h"
#include "../Properties.h"

TAO_Notify_SequenceProxyPushSupplier::TAO_Notify_SequenceProxyPushSupplier (void)
{
}

TAO_Notify_SequenceProxyPushSupplier::~TAO_Notify_SequenceProxyPushSupplier ()
{
}

void
TAO_Notify_SequenceProxyPushSupplier::release (void)
{
  if (this->consumer_)
    this->consumer_->release ();

  delete this;
  //@@ inform factory
}

void
TAO_Notify_SequenceProxyPushSupplier::connect_sequence_push_consumer (CosNotifyComm::SequencePushConsumer_ptr push_consumer ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   , CosEventChannelAdmin::TypeError
                   ))
{
  // Convert Consumer to Base Type
  TAO_Notify_SequencePushConsumer* consumer;
  ACE_NEW_THROW_EX (consumer,
                    TAO_Notify_SequencePushConsumer (this),
                    CORBA::NO_MEMORY ());

  consumer->init (push_consumer, this->admin_properties_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->connect (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_SequenceProxyPushSupplier::disconnect_sequence_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxyType
TAO_Notify_SequenceProxyPushSupplier::MyType (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::PUSH_SEQUENCE;
}

const char *
TAO_Notify_SequenceProxyPushSupplier::get_proxy_type_name (void) const
{
  return "sequence_proxy_push_supplier";
}

void
TAO_Notify_SequenceProxyPushSupplier::load_attrs (const TAO_Notify::NVPList& attrs)
{
  SuperClass::load_attrs(attrs);
  ACE_CString ior;
  if (attrs.load("PeerIOR", ior) && ior.length() > 0)
  {
    CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY
    {
      CORBA::Object_var obj = orb->string_to_object(ior.c_str() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CosNotifyComm::SequencePushConsumer_var pc =
        CosNotifyComm::SequencePushConsumer::_unchecked_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->connect_sequence_push_consumer(pc.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_CATCHANY
    {
      // if we can't reconnect, tough
    }
    ACE_ENDTRY;
  }
}
