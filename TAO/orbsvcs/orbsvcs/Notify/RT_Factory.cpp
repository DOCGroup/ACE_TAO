// $Id$

#include "RT_Factory.h"

#if ! defined (__ACE_INLINE__)
#include "RT_Factory.inl"
#endif /* __ACE_INLINE__ */

#include "Structured/RT_StructuredProxyPushSupplier.h"

ACE_RCSID(RT_Notify, TAO_Notify_RT_Factory, "$Id$")

TAO_Notify_RT_Factory::TAO_Notify_RT_Factory (void)
{
}

TAO_Notify_RT_Factory::~TAO_Notify_RT_Factory ()
{
}

void
TAO_Notify_RT_Factory::create (TAO_Notify_StructuredProxyPushSupplier*& proxy ACE_ENV_ARG_DECL)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_RT_StructuredProxyPushSupplier (),
                    CORBA::NO_MEMORY ());
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_ProxySupplier_Collection*& collection ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (collection ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_ProxyConsumer_Collection*& collection ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (collection ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_EventChannel_Collection*& collection ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (collection ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_ConsumerAdmin_Collection*& collection ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (collection ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_SupplierAdmin_Collection*& collection ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (collection ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_Proxy_Collection*& collection ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (collection ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_EventChannelFactory*& channel_factory ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (channel_factory ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_EventChannel*& channel ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (channel ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_SupplierAdmin*& admin ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (admin ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_ConsumerAdmin*& admin ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (admin ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_ProxyPushConsumer*& proxy ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_ProxyPushSupplier*& proxy ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_CosEC_ProxyPushConsumer*& proxy ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_CosEC_ProxyPushSupplier*& proxy ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_StructuredProxyPushConsumer*& proxy ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_SequenceProxyPushConsumer*& proxy ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_SequenceProxyPushSupplier*& proxy ACE_ENV_ARG_DECL)
{
  TAO_Notify_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

ACE_FACTORY_DEFINE (TAO_RT_Notify, TAO_Notify_RT_Factory)
