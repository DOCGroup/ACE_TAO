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
      : MonitorPoint<ENABLED> ("CPULoad", Datatypes::IT_NUMBER)
    {}
  
    template<bool ENABLED>
    CPULoadMonitor<ENABLED>::~CPULoadMonitor (void)
    {}

    //======================================================
    
    template<bool ENABLED>
    MemoryUsageMonitor<ENABLED>::MemoryUsageMonitor (void)
      : MonitorPoint<ENABLED> ("MemoryUsage", Datatypes::IT_NUMBER)
    {}
  
    template<bool ENABLED>
    MemoryUsageMonitor<ENABLED>::~MemoryUsageMonitor (void)
    {}

    //======================================================
    
    template<bool ENABLED>
    NumThreadsMonitor<ENABLED>::NumThreadsMonitor (void)
      : MonitorPoint<ENABLED> ("NumThreads", Datatypes::IT_NUMBER)
    {}
  
    template<bool ENABLED>
    NumThreadsMonitor<ENABLED>::~NumThreadsMonitor (void)
    {}

    //======================================================
    
    template<bool ENABLED>
    BytesSentMonitor<ENABLED>::BytesSentMonitor (void)
      : MonitorPoint<ENABLED> ("BytesSent", Datatypes::IT_COUNTER)
    {}
  
    template<bool ENABLED>
    BytesSentMonitor<ENABLED>::~BytesSentMonitor (void)
    {}

    //======================================================
    
    template<bool ENABLED>
    PacketsSentMonitor<ENABLED>::PacketsSentMonitor (void)
      : MonitorPoint<ENABLED> ("PacketsSent", Datatypes::IT_COUNTER)
    {}
  
    template<bool ENABLED>
    PacketsSentMonitor<ENABLED>::~PacketsSentMonitor (void)
    {}
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif // LOWLEVELMONITORS_T_CPP

