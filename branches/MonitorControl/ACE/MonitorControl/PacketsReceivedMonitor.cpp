// $Id$

#include "MonitorControl/PacketsReceivedMonitor.h"

#if defined (ACE_ENABLE_MONITORS)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    PacketsReceivedMonitor::PacketsReceivedMonitor (void)
      : Monitor_Base ("OS/Network/PacketsReceived")
#if defined (ACE_WIN32)
        , WindowsMultiInstanceMonitor (
            "\\Network Interface(*)\\Packets Received/sec")
#elif defined (linux)
        , LinuxNetworkInterfaceMonitor (
            " %*[^:]: %*u %lu %*u %*u %*u %*u %*u %*u %*u %*u")
#endif
    {}

    void
    PacketsReceivedMonitor::update (void)
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

#endif /* ACE_ENABLE_MONITORS */

