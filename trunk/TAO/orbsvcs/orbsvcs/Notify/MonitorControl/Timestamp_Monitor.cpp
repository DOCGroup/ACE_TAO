// $Id$

#include "orbsvcs/orbsvcs/Notify/MonitorControl/Timestamp_Monitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

Timestamp_Monitor::Timestamp_Monitor (const char *name)
  : Monitor_Base (name, Monitor_Base::MC_TIME)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */
