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

ACE_FACTORY_DEFINE (TAO_Notify, TAO_NS_RT_Factory)
