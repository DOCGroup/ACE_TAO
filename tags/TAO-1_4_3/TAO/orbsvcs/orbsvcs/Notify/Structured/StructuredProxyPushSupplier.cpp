// $Id$

#include "StructuredProxyPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include "tao/PortableServer/Servant_Base.h"
#include "tao/debug.h"

#include "StructuredPushConsumer.h"
#include "../Properties.h"


ACE_RCSID(RT_Notify, TAO_Notify_StructuredProxyPushSupplier, "$Id$")

TAO_Notify_StructuredProxyPushSupplier::TAO_Notify_StructuredProxyPushSupplier (void)
{
}

TAO_Notify_StructuredProxyPushSupplier::~TAO_Notify_StructuredProxyPushSupplier ()
{
}

void
TAO_Notify_StructuredProxyPushSupplier::release (void)
{
  if (this->consumer_)
    this->consumer_->release ();

  delete this;
  //@@ inform factory
}

CosNotifyChannelAdmin::ProxyType
TAO_Notify_StructuredProxyPushSupplier::MyType (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::PUSH_STRUCTURED;
}

void
TAO_Notify_StructuredProxyPushSupplier::connect_structured_push_consumer (CosNotifyComm::StructuredPushConsumer_ptr push_consumer ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   , CosEventChannelAdmin::TypeError
                   ))
{
  // Convert Consumer to Base Type
  TAO_Notify_StructuredPushConsumer* consumer;
  ACE_NEW_THROW_EX (consumer,
                    TAO_Notify_StructuredPushConsumer (this),
                    CORBA::NO_MEMORY ());

  consumer->init (push_consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->connect (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_StructuredProxyPushSupplier::disconnect_structured_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

const char *
TAO_Notify_StructuredProxyPushSupplier::get_proxy_type_name (void) const
{
  return "structured_proxy_push_supplier";
}

void
TAO_Notify_StructuredProxyPushSupplier::load_attrs (const TAO_Notify::NVPList& attrs)
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
      CosNotifyComm::StructuredPushConsumer_var pc =
        CosNotifyComm::StructuredPushConsumer::_unchecked_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->connect_structured_push_consumer(pc.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_CATCHANY
    {
      // if we can't connect... tough
    }
    ACE_ENDTRY;
  }
}
