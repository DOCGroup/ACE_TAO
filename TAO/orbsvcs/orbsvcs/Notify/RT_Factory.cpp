// $Id$

#include "orbsvcs/Notify/RT_Factory.h"
#include "orbsvcs/Notify/Structured/RT_StructuredProxyPushSupplier.h"

ACE_RCSID(RT_Notify, TAO_Notify_RT_Factory, "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_RT_Factory::TAO_Notify_RT_Factory (void)
{
}

TAO_Notify_RT_Factory::~TAO_Notify_RT_Factory ()
{
}

void
TAO_Notify_RT_Factory::create (TAO_Notify_StructuredProxyPushSupplier*& proxy)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_RT_StructuredProxyPushSupplier (),
                    CORBA::NO_MEMORY ());
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_ProxySupplier_Collection*& collection)
{
  TAO_Notify_Default_Factory::create (collection);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_ProxyConsumer_Collection*& collection)
{
  TAO_Notify_Default_Factory::create (collection);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_EventChannel_Collection*& collection)
{
  TAO_Notify_Default_Factory::create (collection);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_ConsumerAdmin_Collection*& collection)
{
  TAO_Notify_Default_Factory::create (collection);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_SupplierAdmin_Collection*& collection)
{
  TAO_Notify_Default_Factory::create (collection);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_Proxy_Collection*& collection)
{
  TAO_Notify_Default_Factory::create (collection);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_EventChannelFactory*& channel_factory)
{
  TAO_Notify_Default_Factory::create (channel_factory);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_EventChannel*& channel)
{
  TAO_Notify_Default_Factory::create (channel);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_SupplierAdmin*& admin)
{
  TAO_Notify_Default_Factory::create (admin);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_ConsumerAdmin*& admin)
{
  TAO_Notify_Default_Factory::create (admin);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_ProxyPushConsumer*& proxy)
{
  TAO_Notify_Default_Factory::create (proxy);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_ProxyPushSupplier*& proxy)
{
  TAO_Notify_Default_Factory::create (proxy);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_CosEC_ProxyPushConsumer*& proxy)
{
  TAO_Notify_Default_Factory::create (proxy);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_CosEC_ProxyPushSupplier*& proxy)
{
  TAO_Notify_Default_Factory::create (proxy);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_StructuredProxyPushConsumer*& proxy)
{
  TAO_Notify_Default_Factory::create (proxy);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_SequenceProxyPushConsumer*& proxy)
{
  TAO_Notify_Default_Factory::create (proxy);
}

void
TAO_Notify_RT_Factory:: create (TAO_Notify_SequenceProxyPushSupplier*& proxy)
{
  TAO_Notify_Default_Factory::create (proxy);
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_RT_Notify, TAO_Notify_RT_Factory)
