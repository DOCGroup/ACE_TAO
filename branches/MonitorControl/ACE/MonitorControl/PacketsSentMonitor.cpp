// $Id$

#include "MonitorControl/PacketsSentMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    PacketsSentMonitor<true>::PacketsSentMonitor (void)
      : MonitorPoint<true> ("PacketsSent")
#if defined (ACE_WIN32)
      , WindowsMultiInstanceMonitor (
          "\\Network Interface(*)\\Packets Sent/sec")
#elif defined (linux)
        , LinuxNetworkInterfaceMonitor (
            " %*[^:]: %*u %*u %*u %*u %*u %*u %*u %*u %*u %lu")
#endif
    {}

    void
    PacketsSentMonitor<true>::update (void)
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

