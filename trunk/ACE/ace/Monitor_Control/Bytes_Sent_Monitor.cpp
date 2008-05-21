// $Id$

#include "ace/Monitor_Control/Bytes_Sent_Monitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    const char* Bytes_Sent_Monitor::default_name_ =
      "OS/Network/BytesSent";

    Bytes_Sent_Monitor::Bytes_Sent_Monitor (const char* name)
      : Monitor_Base (name)
#if defined (ACE_HAS_WIN32_PDH)
        , Windows_Multi_Instance_Monitor (
            ACE_TEXT ("\\Network Interface(*)\\Bytes Sent/sec"))
#elif defined (linux)
        , Linux_Network_Interface_Monitor (
            " %*[^:]: %*u %*u %*u %*u %*u %*u %*u %*u %lu %*u")
            /// Scan format for /proc/net/dev
#endif
    {}

    void
    Bytes_Sent_Monitor::update (void)
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
    Bytes_Sent_Monitor::default_name (void)
    {
      return Bytes_Sent_Monitor::default_name_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */


