// $Id$

#include "MonitorControl/CPULoadMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    void
    CPULoadMonitor<true>::update (void)
    {
      // TODO
      ACE_DEBUG ((LM_DEBUG, "update\n"));
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

