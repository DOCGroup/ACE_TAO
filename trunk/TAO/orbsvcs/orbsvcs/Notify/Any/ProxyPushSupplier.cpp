// $Id$

#include "orbsvcs/Notify/Any/ProxyPushSupplier.h"

ACE_RCSID (Notify, TAO_Notify_ProxyPushSupplier, "$Id$")

#include "tao/debug.h"
#include "orbsvcs/Notify/Any/PushConsumer.h"
#include "orbsvcs/Notify/Properties.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_ProxyPushSupplier::TAO_Notify_ProxyPushSupplier (void)
{
}

TAO_Notify_ProxyPushSupplier::~TAO_Notify_ProxyPushSupplier ()
{
}

void
TAO_Notify_ProxyPushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_ProxyPushSupplier::connect_any_push_consumer (CosEventComm::PushConsumer_ptr push_consumer
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
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_ProxyPushSupplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_ProxyPushSupplier::Ptr guard( this );
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxyType
TAO_Notify_ProxyPushSupplier::MyType (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::PUSH_ANY;
}

const char *
TAO_Notify_ProxyPushSupplier::get_proxy_type_name (void) const
{
  return "proxy_push_supplier";
}

void
TAO_Notify_ProxyPushSupplier::load_attrs (const TAO_Notify::NVPList& attrs)
{
  SuperClass::load_attrs(attrs);
  ACE_CString ior;
  if (attrs.load("PeerIOR", ior))
    {
      CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          CosNotifyComm::PushConsumer_var pc =
            CosNotifyComm::PushConsumer::_nil();
          if (ior.length() > 0)
            {
              CORBA::Object_var obj =
                orb->string_to_object(ior.c_str() ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              pc = CosNotifyComm::PushConsumer::_unchecked_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          this->connect_any_push_consumer(pc.in() ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // if we can't reconnect, tough...
        }
      ACE_ENDTRY;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
