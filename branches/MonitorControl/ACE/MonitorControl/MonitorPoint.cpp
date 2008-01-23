// $Id$

#include "MonitorControl/MonitorPoint.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    void
    MonitorPoint<true>::group_parent (const Statistic* /* parent */)
    {
      // TODO
    }
    
    void
    MonitorPoint<true>::add_group_child (const Statistic* /* child */)
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
    MonitorPoint<true>::receive (const double /* data */)
    {
    }
    
    void
    MonitorPoint<true>::receive (const Datatypes::NameList& /* data */)
    {
      // TODO
    }
    
    void
    MonitorPoint<true>::clear (void)
    {
      // TODO
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

