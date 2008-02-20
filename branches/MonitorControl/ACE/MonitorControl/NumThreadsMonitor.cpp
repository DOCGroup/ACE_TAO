// $Id$

#if defined (ACE_HAS_KSTAT)
#include <sys/sysinfo.h>
#endif

#include "MonitorControl/NumThreadsMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    NumThreadsMonitor<true>::NumThreadsMonitor (void)
      : MonitorPoint<true> ("NumThreads")
#if defined (ACE_WIN32)
      , WindowsMonitor ("\\System\\Threads")
#endif
    {
    }
  
    void
    NumThreadsMonitor<true>::update (void)
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

