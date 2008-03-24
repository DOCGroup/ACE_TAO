// $Id$

#include "ace/CDR_Buffer_Monitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    CDR_Buffer_Monitor::CDR_Buffer_Monitor (void)
      : Size_Monitor ("CDR Buffer Monitor")
    {
    }
        
    CDR_Buffer_Monitor::CDR_Buffer_Monitor (const char* name)
      : Size_Monitor (name)
    {
    }
    
    CDR_Buffer_Monitor::~CDR_Buffer_Monitor (void)
    {
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
