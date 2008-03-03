// $Id$

#ifndef LOWLEVELMONITORS_T_CPP
#define LOWLEVELMONITORS_T_CPP

#include "MonitorControl/LowLevelMonitors_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// TODO - replace hard-coded name strings with actual platform-dependent
//        names, via macros or string constants.

namespace ACE
{
  namespace MonitorControl
  {
    template<bool ENABLED>
    CPULoadMonitor<ENABLED>::CPULoadMonitor (void)
      : MonitorPoint<ENABLED> ("CPULoad")
    {}
  
    template<bool ENABLED>
    CPULoadMonitor<ENABLED>::~CPULoadMonitor (void)
    {}

    //======================================================
    
    template<bool ENABLED>
    MemoryUsageMonitor<ENABLED>::MemoryUsageMonitor (void)
      : MonitorPoint<ENABLED> ("MemoryUsage")
    {}
  
    template<bool ENABLED>
    MemoryUsageMonitor<ENABLED>::~MemoryUsageMonitor (void)
    {}

    //======================================================
    
    template<bool ENABLED>
    NumThreadsMonitor<ENABLED>::NumThreadsMonitor (void)
      : MonitorPoint<ENABLED> ("NumThreads")
    {}
  
    template<bool ENABLED>
    NumThreadsMonitor<ENABLED>::~NumThreadsMonitor (void)
    {}

    //======================================================
    
    template<bool ENABLED>
    BytesSentMonitor<ENABLED>::BytesSentMonitor (void)
      : MonitorPoint<ENABLED> ("BytesSent")
    {}
  
    template<bool ENABLED>
    BytesSentMonitor<ENABLED>::~BytesSentMonitor (void)
    {}

    //======================================================
    
    template<bool ENABLED>
    BytesReceivedMonitor<ENABLED>::BytesReceivedMonitor (void)
      : MonitorPoint<ENABLED> ("BytesReceived")
    {}
  
    template<bool ENABLED>
    BytesReceivedMonitor<ENABLED>::~BytesReceivedMonitor (void)
    {}

    //======================================================
    
    template<bool ENABLED>
    PacketsSentMonitor<ENABLED>::PacketsSentMonitor (void)
      : MonitorPoint<ENABLED> ("PacketsSent")
    {}
  
    template<bool ENABLED>
    PacketsSentMonitor<ENABLED>::~PacketsSentMonitor (void)
    {}

    //======================================================
    
    template<bool ENABLED>
    PacketsReceivedMonitor<ENABLED>::PacketsReceivedMonitor (void)
      : MonitorPoint<ENABLED> ("PacketsReceived")
    {}
  
    template<bool ENABLED>
    PacketsReceivedMonitor<ENABLED>::~PacketsReceivedMonitor (void)
    {}
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif // LOWLEVELMONITORS_T_CPP

