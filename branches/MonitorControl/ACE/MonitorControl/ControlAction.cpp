// $Id$

#include "MonitorControl/ControlAction.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    ControlAction::ControlAction (void)
    {}
  
    ControlAction::ControlAction (const char* name)
      : Generic (name)
    {}
    
    ControlAction::~ControlAction (void)
    {}
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

