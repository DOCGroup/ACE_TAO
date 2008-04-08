// $Id$

#include "ace/Guard_T.h"
#include "ace/Control_Action.h"

#include "MonitorControl/MonitorPoint.h"
#include "MonitorControl/Constraint_Interpreter.h"
#include "MonitorControl/Constraint_Visitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MonitorPoint<true>::MonitorPoint (const char* name)
      : Monitor_Base (name)
    {}
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

