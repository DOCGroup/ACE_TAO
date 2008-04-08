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
    MonitorPoint<ENABLED>::MonitorPoint (const char* name)
      : Monitor_Base (name)
    {}

    template<bool ENABLED>
    MonitorPoint<ENABLED>::~MonitorPoint (void)
    {}

    template<bool ENABLED>
    void
    MonitorPoint<ENABLED>::constraint (
      const char* /* constraint */)
    {
    }

    template<bool ENABLED>
    void
    MonitorPoint<ENABLED>::control_action (Control_Action* /* action */,
                                           const char* /* command */)
    {
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif // MONITORPOINT_T_CPP

