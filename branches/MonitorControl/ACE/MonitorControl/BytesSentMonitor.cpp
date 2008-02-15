// $Id$

#include "MonitorControl/BytesSentMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    BytesSentMonitor<true>::BytesSentMonitor (void)
      : MonitorPoint<true> ("BytesSent",
                            MonitorControl_Types::IT_NUMBER)
    {}
  
    void
    BytesSentMonitor<true>::update (void)
    {
      // TODO
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

