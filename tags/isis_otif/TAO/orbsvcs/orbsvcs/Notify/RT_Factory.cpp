// $Id$

#include "RT_Factory.h"

#if ! defined (__ACE_INLINE__)
#include "RT_Factory.inl"
#endif /* __ACE_INLINE__ */

#include "Structured/RT_StructuredProxyPushSupplier.h"

ACE_RCSID(RT_Notify, TAO_NS_RT_Factory, "$Id$")

TAO_NS_RT_Factory::TAO_NS_RT_Factory (void)
{
}

TAO_NS_RT_Factory::~TAO_NS_RT_Factory ()
{
}

void
TAO_NS_RT_Factory::create (TAO_NS_StructuredProxyPushSupplier*& proxy ACE_ENV_ARG_DECL)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_NS_RT_StructuredProxyPushSupplier (),
                    CORBA::NO_MEMORY ());
}

void
TAO_NS_RT_Factory:: create (TAO_NS_ProxySupplier_Collection*& collection ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (collection ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_ProxyConsumer_Collection*& collection ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (collection ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_EventChannel_Collection*& collection ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (collection ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_ConsumerAdmin_Collection*& collection ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (collection ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_SupplierAdmin_Collection*& collection ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (collection ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_Proxy_Collection*& collection ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (collection ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_EventChannelFactory*& channel_factory ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (channel_factory ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_EventChannel*& channel ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (channel ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_SupplierAdmin*& admin ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (admin ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_ConsumerAdmin*& admin ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (admin ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_ProxyPushConsumer*& proxy ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_ProxyPushSupplier*& proxy ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_CosEC_ProxyPushConsumer*& proxy ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_CosEC_ProxyPushSupplier*& proxy ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_StructuredProxyPushConsumer*& proxy ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_SequenceProxyPushConsumer*& proxy ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Factory:: create (TAO_NS_SequenceProxyPushSupplier*& proxy ACE_ENV_ARG_DECL)
{
  TAO_NS_Default_Factory::create (proxy ACE_ENV_ARG_PARAMETER);
}

ACE_FACTORY_DEFINE (TAO_RT_Notify, TAO_NS_RT_Factory)
