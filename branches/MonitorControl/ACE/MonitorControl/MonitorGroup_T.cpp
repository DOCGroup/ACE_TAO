// $Id$

#ifndef MONITORGROUP_T_CPP
#define MONITORGROUP_T_CPP

#include "MonitorControl/MonitorGroup_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    template<bool ENABLED>
    MonitorGroup<ENABLED>::MonitorGroup (const char* name)
      : Monitor_Base (name)
    {}
    
    template<bool ENABLED>
    void
    MonitorGroup<ENABLED>::constraint (
      const ETCL_Constraint* /* constraint */)
    {
    }
    
    template<bool ENABLED>
    void
    MonitorGroup<ENABLED>::receive (double /* data */)
    {
    }
    
    template<bool ENABLED>
    void
    MonitorGroup<ENABLED>::receive (size_t /* data */)
    {
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif // MONITORGROUP_T_CPP

