// $Id$

#include "RTCORBA_StructuredProxyPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "RTCORBA_StructuredProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_RTCORBA_StructuredProxyPushSupplier, "$Id$")

#include "tao/debug.h"
#include "StructuredPushConsumer.h"
#include "StructuredEvent.h"
#include "../Worker_Task.h"
#include "../RTCORBA_Properties.h"
#include "../Method_Request_Dispatch.h"
#include "../Method_Request_Dispatch_No_Filtering.h"
#include "../Event_Manager.h"
#include "../Proxy.h"
#include "../Admin.h"
#include "../EventChannel.h"
#include "../EventChannelFactory.h"
#include "../Notify_Service.h"

TAO_NS_RTCORBA_StructuredProxyPushSupplier::TAO_NS_RTCORBA_StructuredProxyPushSupplier (void)
{
}

TAO_NS_RTCORBA_StructuredProxyPushSupplier::~TAO_NS_RTCORBA_StructuredProxyPushSupplier ()
{
}

void
TAO_NS_RTCORBA_StructuredProxyPushSupplier::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "In TAO_NS_RTCORBA_StructuredProxyPushConsumer::destroy \n"));

  this->inherited::destroy (this ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RTCORBA_StructuredProxyPushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}
