// $Id$

#include "MonitorControl/PacketsReceivedMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    PacketsReceivedMonitor<true>::PacketsReceivedMonitor (void)
      : MonitorPoint<true> ("PacketsReceived")
#if defined (ACE_WIN32)
      , WindowsMultiInstanceMonitor (
          "\\Network Interface(*)\\Packets Received/sec")
#endif
    {}

    void
    PacketsReceivedMonitor<true>::update (void)
    {
#if defined (ACE_WIN32)
      this->win_update ();

      /// Stores value and timestamp with thread-safety.
      this->receive (this->value_.doubleValue);
    }
#endif
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

