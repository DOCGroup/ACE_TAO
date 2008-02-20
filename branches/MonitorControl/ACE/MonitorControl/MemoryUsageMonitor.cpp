// $Id$

#if defined (ACE_HAS_KSTAT)
#include <sys/sysinfo.h>
#endif

#include "MonitorControl/MemoryUsageMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MemoryUsageMonitor<true>::MemoryUsageMonitor (void)
      : MonitorPoint<true> ("MemoryUsage")
#if defined (ACE_WIN32)
      , WindowsMonitor ("\\Memory\\% Committed Bytes In Use")
#elif defined (linux)
      , file_ptr_ (0)
#elif defined (ACE_HAS_KSTAT)
      , kstats_ (0)
      , kstat_ (0)
      , kstat_id_ (0)
#endif
    {
    }
  
    void
    MemoryUsageMonitor<true>::update (void)
    {
#if defined (ACE_WIN32)
      this->win_update ();

      /// Stores value and timestamp with thread-safety.
      this->receive (this->value_.doubleValue);
#elif defined (linux)
#elif defined (ACE_HAS_KSTAT)
#endif
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

