// $Id$

#include "RTCORBA_Factory.h"
#include "Structured/RTCORBA_StructuredProxyPushSupplier_Tie.h"
#include "Structured/RTCORBA_StructuredProxyPushConsumer.h"
#include "RTCORBA_EventChannel.h"
#include "Admin.h"
#include "Proxy.h"
#include "RTCORBA_QoSAdmin.h"

#if ! defined (__ACE_INLINE__)
#include "RTCORBA_Factory.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_RTCORBA_Factory, "$Id$")

TAO_NS_RTCORBA_Factory::TAO_NS_RTCORBA_Factory (void)
{
}

TAO_NS_RTCORBA_Factory::~TAO_NS_RTCORBA_Factory ()
{
}

void
TAO_NS_RTCORBA_Factory::create (TAO_NS_QoSAdmin*& qos_admin ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (qos_admin, TAO_NS_RTCORBA_QoSAdmin (), CORBA::NO_MEMORY ());
}

void
TAO_NS_RTCORBA_Factory::create (TAO_NS_EventChannel*& channel ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (channel,
                    TAO_NS_RTCORBA_EventChannel (),
                    CORBA::NO_MEMORY ());
}

void
TAO_NS_RTCORBA_Factory::create (TAO_NS_StructuredProxyPushSupplier*& proxy ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_RTCORBA_StructuredProxyPushSupplier_Tie* proxy_tie;

  ACE_NEW_THROW_EX (proxy_tie,
                    TAO_NS_RTCORBA_StructuredProxyPushSupplier_Tie (),
                    CORBA::NO_MEMORY ());
  proxy = proxy_tie->_tied_object ();

  /// set correct callback.
  proxy->destroy_callback (proxy_tie);

}

void
TAO_NS_RTCORBA_Factory::create (TAO_NS_StructuredProxyPushConsumer*& proxy ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_NS_RTCORBA_StructuredProxyPushConsumer (),
                    CORBA::NO_MEMORY ());
}
