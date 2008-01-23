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
                                         Datatypes::InfoType /* type */)
      : Statistic (name)
    {}
    
    template<bool ENABLED>
    MonitorPoint<ENABLED>::~MonitorPoint (void)
    {}
    
    template<bool ENABLED>
    void
    MonitorPoint<ENABLED>::group_parent (const Statistic* /* parent */)
    {
    }
    
    template<bool ENABLED>
    void
    MonitorPoint<ENABLED>::add_group_child (const Statistic* /* child */)
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
    MonitorPoint<ENABLED>::receive (const double /* data */)
    {
    }
    
    template<bool ENABLED>
    void
    MonitorPoint<ENABLED>::receive (const Datatypes::NameList& /* data */)
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

