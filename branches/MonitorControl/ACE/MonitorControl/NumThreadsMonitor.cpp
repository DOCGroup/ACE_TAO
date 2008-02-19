// $Id$

#include "MonitorControl/NumThreadsMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    NumThreadsMonitor<true>::NumThreadsMonitor (void)
      : MonitorPoint<true> ("NumThreads")
    {}
  
    void
    NumThreadsMonitor<true>::update (void)
    {
      // TODO
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

