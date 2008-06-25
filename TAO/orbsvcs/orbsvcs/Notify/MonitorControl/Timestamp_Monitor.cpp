// $Id$

#include "orbsvcs/orbsvcs/Notify/MonitorControl/Timestamp_Monitor.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

Timestamp_Monitor::Timestamp_Monitor (const char *name)
  : Monitor_Base (name, Monitor_Base::MC_TIME)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */
