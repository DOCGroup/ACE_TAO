// $Id$

#ifndef MONITORPOINT_T_CPP
#define MONITORPOINT_T_CPP

#include "MonitorControl/MonitorPoint_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    template<bool ENABLED>
    MonitorPoint<ENABLED>::MonitorPoint (const char* name,
                                         MonitorControl_Types::InfoType /* type */)
      : Monitor_Base (name)
    {}
    
    template<bool ENABLED>
    MonitorPoint<ENABLED>::~MonitorPoint (void)
    {}
    
    template<bool ENABLED>
    void
    MonitorPoint<ENABLED>::group_parent (const Monitor_Base* /* parent */)
    {
    }
    
    template<bool ENABLED>
    void
    MonitorPoint<ENABLED>::add_group_child (const Monitor_Base* /* child */)
    {
    }
    
    template<bool ENABLED>
    void
    MonitorPoint<ENABLED>::constraint (
      const ETCL_Constraint* /* constraint */)
    {
    }
    
    template<bool ENABLED>
    void
    MonitorPoint<ENABLED>::receive (double /* data */)
    {
    }
    
    template<bool ENABLED>
    void
    MonitorPoint<ENABLED>::clear (void)
    {
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif // MONITORPOINT_T_CPP

