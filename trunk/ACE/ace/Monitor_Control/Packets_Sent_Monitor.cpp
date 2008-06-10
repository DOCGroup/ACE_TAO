// $Id$

#include "ace/Monitor_Control/Packets_Sent_Monitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    const char* Packets_Sent_Monitor::default_name_ =
      "OS/Network/PacketsSent";

    Packets_Sent_Monitor::Packets_Sent_Monitor (const char* name)
      : Monitor_Base (name)
#if defined (ACE_HAS_WIN32_PDH)
      , Windows_Multi_Instance_Monitor (
          ACE_TEXT("\\Network Interface(*)\\Packets Sent/sec"))
#elif defined (linux)
        , Linux_Network_Interface_Monitor (
            " %*[^:]: %*u %*u %*u %*u %*u %*u %*u %*u %*u %lu")
            /// Scan format for /proc/net/dev
#endif
    {}

    void
    Packets_Sent_Monitor::update (void)
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
    Packets_Sent_Monitor::default_name (void)
    {
      return Packets_Sent_Monitor::default_name_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

