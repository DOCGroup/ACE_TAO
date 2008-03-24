// $Id$

#include "ace/Message_Queue_Monitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Message_Queue_Monitor::Message_Queue_Monitor (void)
      : Size_Monitor ("Message Queue Monitor")
    {
    }
        
    Message_Queue_Monitor::Message_Queue_Monitor (const char* name)
      : Size_Monitor (name)
    {
    }
    
    Message_Queue_Monitor::~Message_Queue_Monitor (void)
    {
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
