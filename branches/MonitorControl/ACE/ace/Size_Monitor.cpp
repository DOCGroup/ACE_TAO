// $Id$

#include "ace/Size_Monitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK)

#include "ace/Guard_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Size_Monitor::Size_Monitor (void)
    {
    }

    Size_Monitor::Size_Monitor (const char* name)
      : Monitor_Base (name)
    {
    }

    Size_Monitor::~Size_Monitor (void)
    {
    }

    void
    Size_Monitor::update (void)
    {
      // No platform-specific or periodic code is needed, receive() can be
      // called directly whenever the size changes.
    }

    void
    Size_Monitor::clear (void)
    {
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK*/

