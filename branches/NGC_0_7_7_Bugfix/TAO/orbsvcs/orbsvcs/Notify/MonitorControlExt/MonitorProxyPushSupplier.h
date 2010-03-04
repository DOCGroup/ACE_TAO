// $Id$
#ifndef MONITORPROXYPUSHSUPPLIER_H
#define MONITORPROXYPUSHSUPPLIER_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControlExt/notify_mc_ext_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "orbsvcs/Notify/Any/ProxyPushSupplier.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorProxySupplier_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

class TAO_Notify_MC_Ext_Export TAO_MonitorProxyPushSupplier
  : public TAO_MonitorProxySupplier_T<TAO_Notify_ProxyPushSupplier>
{
  // No implementation needed at this level
  // we're just mixing in the MonitorProxySupplier
};
TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAO_HAS_MONITOR_FRAMEWORK == 1

#include /**/ "ace/post.h"
#endif /* MONITORPROXYPUSHSUPPLIER_H */
