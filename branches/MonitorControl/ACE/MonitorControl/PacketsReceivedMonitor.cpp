// $Id$

#include "MonitorControl/PacketsReceivedMonitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    const char* PacketsReceivedMonitor::default_name_ =
      "OS/Network/PacketsReceived";

    PacketsReceivedMonitor::PacketsReceivedMonitor (const char* name)
      : Monitor_Base (name)
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

    const char*
    PacketsReceivedMonitor::default_name (void)
    {
      return PacketsReceivedMonitor::default_name_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK*/

