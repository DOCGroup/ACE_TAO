// $Id$

#include "MonitorControl/PacketsReceivedMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    PacketsReceivedMonitor<true>::PacketsReceivedMonitor (void)
      : MonitorPoint<true> ("OS/Network/PacketsReceived")
#if defined (ACE_WIN32)
        , WindowsMultiInstanceMonitor (
            "\\Network Interface(*)\\Packets Received/sec")
#elif defined (linux)
        , LinuxNetworkInterfaceMonitor (
            " %*[^:]: %*u %lu %*u %*u %*u %*u %*u %*u %*u %*u")
#endif
    {}

    void
    PacketsReceivedMonitor<true>::update (void)
    {
#if defined (ACE_WIN32)
      this->win_update ();

      /// Stores value and timestamp with thread-safety.
      this->receive (this->value_.doubleValue);
#elif defined (linux)
      this->lin_update ();

      /// Stores value and timestamp with thread-safety.
      this->receive (static_cast<double> (this->value_));
#endif
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

