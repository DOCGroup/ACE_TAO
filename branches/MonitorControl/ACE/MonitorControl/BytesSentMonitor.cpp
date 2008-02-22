// $Id$

#include "MonitorControl/BytesSentMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    BytesSentMonitor<true>::BytesSentMonitor (void)
      : MonitorPoint<true> ("BytesSent")
#if defined (ACE_WIN32)
      , WindowsMultiInstanceMonitor ("\\Network Interface(*)\\Bytes Total/sec")
#endif
    {}

    void
    BytesSentMonitor<true>::update (void)
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

