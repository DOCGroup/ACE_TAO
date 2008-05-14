// $Id$

#include "ace/MonitorControl/PacketsReceivedMonitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    const char* PacketsReceivedMonitor::default_name_ =
      "OS/Network/PacketsReceived";

    PacketsReceivedMonitor::PacketsReceivedMonitor (const char* name)
      : Monitor_Base (name)
#if defined (ACE_HAS_WIN32_PDH)
        , WindowsMultiInstanceMonitor (
            ACE_TEXT ("\\Network Interface(*)\\Packets Received/sec"))
#elif defined (linux)
        , LinuxNetworkInterfaceMonitor (
            " %*[^:]: %*u %lu %*u %*u %*u %*u %*u %*u %*u %*u")
            /// Scan format for /proc/net/dev
#endif
    {}

    void
    PacketsReceivedMonitor::update (void)
    {
#if defined (ACE_HAS_WIN32_PDH)
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

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

