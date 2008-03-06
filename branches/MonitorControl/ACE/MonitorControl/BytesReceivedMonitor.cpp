// $Id$

#include "MonitorControl/BytesReceivedMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    BytesReceivedMonitor<true>::BytesReceivedMonitor (void)
      : MonitorPoint<true> ("BytesReceived")
#if defined (ACE_WIN32)
        , WindowsMultiInstanceMonitor (
            "\\Network Interface(*)\\Bytes Received/sec")
#elif defined (linux)
        , LinuxNetworkInterfaceMonitor (
            " %*[^:]: %lu %*u %*u %*u %*u %*u %*u %*u %*u %*u")
#endif
    {}

    void
    BytesReceivedMonitor<true>::update (void)
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

