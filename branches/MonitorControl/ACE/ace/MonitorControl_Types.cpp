// $Id$

#include "ace/MonitorControl_Types.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MonitorControl_Types::Data::Data (void)
      : timestamp_ (ACE_Time_Value::zero),
        value_ (0.0)
    {}
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

