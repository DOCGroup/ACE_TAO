// $Id$

#include "MonitorControl/MonitorPoint.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MonitorPoint<true>::MonitorPoint (
      const char* name,
      MonitorControl_Types::InfoType /* type */)
      : Monitor_Base (name)
    {}
    
    void
    MonitorPoint<true>::group_parent (const Monitor_Base* /* parent */)
    {
      // TODO
    }
    
    void
    MonitorPoint<true>::add_group_child (const Monitor_Base* /* child */)
    {
      // TODO
    }
    
    void
    MonitorPoint<true>::constraint (
      const ETCL_Constraint* /* constraint */)
    {
      // TODO
    }
    
    void
    MonitorPoint<true>::receive (double /* data */)
    {
    }
    
    void
    MonitorPoint<true>::receive (size_t /* data */)
    {
    }
    
    void
    MonitorPoint<true>::clear (void)
    {
      // TODO
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

