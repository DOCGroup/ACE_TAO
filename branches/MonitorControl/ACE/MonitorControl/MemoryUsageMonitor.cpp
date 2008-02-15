// $Id$

#include "MonitorControl/MemoryUsageMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MemoryUsageMonitor<true>::MemoryUsageMonitor (void)
      : MonitorPoint<true> ("MemoryUsage",
                            MonitorControl_Types::IT_NUMBER)
    {}
  
    void
    MemoryUsageMonitor<true>::update (void)
    {
      // TODO
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

