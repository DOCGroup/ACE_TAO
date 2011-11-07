// $Id$
#ifndef MONITORSEQUENCEPROXYPUSHSUPPLIER_H
#define MONITORSEQUENCEPROXYPUSHSUPPLIER_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControlExt/notify_mc_ext_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "orbsvcs/Notify/Sequence/SequenceProxyPushSupplier.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorProxySupplier_T.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Ext_Export TAO_MonitorSequenceProxyPushSupplier
  : public TAO_MonitorProxySupplier_T<TAO_Notify_SequenceProxyPushSupplier>
{
  // No implementation needed at this level
  // we're just mixing in the MonitorProxySupplier
};
TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAO_HAS_MONITOR_FRAMEWORK == 1

#include /**/ "ace/post.h"
#endif /* MONITORSEQUENCEPROXYPUSHSUPPLIER_H */
