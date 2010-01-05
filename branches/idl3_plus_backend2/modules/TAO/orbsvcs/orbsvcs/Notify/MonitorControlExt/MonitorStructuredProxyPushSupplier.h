// $Id$
#ifndef MONITORSTRUCTUREDPROXYPUSHSUPPLIER_H
#define MONITORSTRUCTUREDPROXYPUSHSUPPLIER_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControlExt/notify_mc_ext_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Structured/StructuredProxyPushSupplier.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorProxySupplier_T.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef TAO_MonitorProxySupplier_T<TAO_Notify_StructuredProxyPushSupplier> TAO_MonitorStructuredProxyPushSupplier;
#if 0
class TAO_Notify_MC_Ext_Export TAO_MonitorStructuredProxyPushSupplier
  : public TAO_MonitorProxySupplier_T<TAO_Notify_StructuredProxyPushSupplier>
{
  // Consider making this a typedef
};
#endif

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAO_HAS_MONITOR_FRAMEWORK == 1

#include /**/ "ace/post.h"
#endif /* MONITORSTRUCTUREDPROXYPUSHSUPPLIER_H */
